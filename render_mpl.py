#!/usr/bin/python3

# Plot Gerber object stream using matplotlib

import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib as mpl
import gerber
import vertices
import copy

# TODO remove magic 1e6 and fix units/scaling
# TODO move vertex calcs into aperture classes etc
# TODO render holes
# TODO render transforms
# TODO add tests
# TODO render primitive rotations


def render(state):
    patches = []
    for obj in state.objects:
        append_render(patches, obj)
    return mpl.collections.PatchCollection(patches, match_original=True)


def append_render(patches, obj):
    if isinstance(obj, gerber.Draw):
        append_render_draw(patches, obj)
    elif isinstance(obj, gerber.Flash):
        append_render_flash(patches, obj)
    elif isinstance(obj, gerber.Arc):
        append_render_arc(patches, obj)
    elif isinstance(obj, gerber.Region):
        append_render_region(patches, obj)
    elif isinstance(obj, gerber.StepAndRepeat):
        append_render_step_and_repeat(patches, obj)
    else:
        raise TypeError('Unsupported object type for render')


def append_render_draw(patches, obj):
    assert isinstance(obj, gerber.Draw)
    assert isinstance(obj.aperture, gerber.Circle)
    x0, y0 = 1e-6 * np.array(obj.origin)
    x1, y1 = 1e-6 * np.array(obj.endpoint)
    pts = vertices.rounded_line(obj.aperture.diameter, x0, y0, x1, y1)
    patches.append(mpatches.Polygon(pts))


def append_render_arc(patches, obj):
    assert isinstance(obj, gerber.Arc)
    assert isinstance(obj.aperture, gerber.Circle)
    dx, dy = 1e-6 * np.array(obj.offset)
    x1, y1 = 1e-6 * np.array(obj.origin)
    x2, y2 = 1e-6 * np.array(obj.endpoint)
    x0, y0 = x1 + dx, y1 + dy
    pts = vertices.rounded_arc(obj.aperture.diameter, x0, y0, x1, y1, x2, y2)
    vertices.translate(pts, x0, y0)
    patches.append(mpatches.Polygon(pts))


def append_render_region(patches, obj):
    assert isinstance(obj, gerber.Region)
    x = []
    y = []
    for segment in obj.objects:
        if isinstance(segment, gerber.Draw):
            x1, y1 = segment.origin
            x.append(x1)
            y.append(y1)
        elif isinstance(segment, gerber.Arc):
            dx, dy = segment.offset
            x1, y1 = segment.origin
            x0, y0 = x1 + dx, y1 + dy
            x2, y2 = segment.endpoint
            r = np.sqrt((x1-x0)**2 + (y1-y0)**2)
            start_angle = np.rad2deg(np.arctan2(y1-y0, x1-x0))
            end_angle = np.rad2deg(np.arctan2(y2-y0, x2-x0))
            xc, yc = get_poly_circle_vertices((x0, y0), r, start_angle,
                                              end_angle, is_cw=segment.is_cw)
            x.extend(xc)
            y.extend(yc)
        else:
            raise TypeError('Segment not supported')
    xy = 1e-6 * np.vstack([x, y]).T
    patches.append(mpatches.Polygon(xy, fill=False))


def append_render_flash(patches, obj):
    assert isinstance(obj, gerber.Flash)
    assert obj.aperture is not None
    if isinstance(obj.aperture, gerber.Circle):
        pts = vertices.circle(obj.aperture.diameter)
        x0, y0 = 1e-6 * np.array(obj.origin)
        vertices.translate(pts, x0, y0)
        patches.append(mpatches.Polygon(pts))
    elif isinstance(obj.aperture, gerber.Polygon):
        pts = vertices.regular_poly(obj.aperture.outer_diameter,
                                    obj.aperture.vertices)
        x0, y0 = 1e-6 * np.array(obj.origin)
        vertices.translate(pts, x0, y0)
        patches.append(mpatches.Polygon(pts))
    elif isinstance(obj.aperture, gerber.Rectangle):
        pts = vertices.rectangle(obj.aperture.x_size, obj.aperture.y_size)
        x0, y0 = 1e-6 * np.array(obj.origin)
        vertices.translate(pts, x0, y0)
        patches.append(mpatches.Polygon(pts))
    elif isinstance(obj.aperture, gerber.Obround):
        x0, y0 = 1e-6 * np.array(obj.origin)
        if obj.aperture.x_size > obj.aperture.y_size:
            w = obj.aperture.y_size
            x1 = 0.5 * (-obj.aperture.x_size + w)
            x2 = 0.5 * (obj.aperture.x_size - w)
            y1, y2 = 0, 0
        else:
            w = obj.aperture.x_size
            x1, x2 = 0, 0
            y1 = 0.5 * (-obj.aperture.y_size + w)
            y2 = 0.5 * (obj.aperture.y_size - w)
        pts = vertices.rounded_line(w, x1, y1, x2, y2)
        vertices.translate(pts, x0, y0)
        patches.append(mpatches.Polygon(pts))
    elif isinstance(obj.aperture, gerber.Macro):
        append_render_macro(patches, obj)
    elif isinstance(obj.aperture, gerber.BlockAperture):
        block_children = [copy.copy(block) for block in obj.aperture.objects]
        for child in block_children:
            child.translate(obj.origin)
            append_render(patches, child)
    else:
        raise NotImplementedError('Unrecognized Aperture ' +
                                  str(type(obj.aperture)))


def append_render_macro(patches, obj):
    x0, y0 = 1e-6 * np.array(obj.origin)
    for prim in obj.aperture.primitives:
        if isinstance(prim, gerber.MacroCircle):
            pts = vertices.circle(prim.diameter)
            vertices.translate(pts, prim.x + x0, prim.y + y0)
            patches.append(mpatches.Polygon(pts))
        elif isinstance(prim, gerber.MacroVectorLine):
            pts = vertices.thick_line(prim.width,
                                      prim.x1, prim.y1,
                                      prim.x2, prim.y2)
            vertices.translate(pts, x0, y0)
            patches.append(mpatches.Polygon(pts))
        elif isinstance(prim, gerber.MacroCenterLine):
            pts = vertices.rectangle(prim.width, prim.height)
            vertices.translate(pts, prim.x + x0, prim.y + y0)
            patches.append(mpatches.Polygon(pts))
        elif isinstance(prim, gerber.MacroOutline):
            N = int(len(prim.coordinates) / 2)
            pts = np.array(prim.coordinates)
            pts.resize((N, 2))
            vertices.translate(pts, x0, y0)
            patches.append(mpatches.Polygon(pts))
        elif isinstance(prim, gerber.MacroPolygon):
            pts = vertices.regular_poly(prim.diameter, prim.vertices)
            vertices.translate(pts, prim.x + x0, prim.y + y0)
            patches.append(mpatches.Polygon(pts))
        elif isinstance(prim, gerber.MacroThermal):
            pts_o = vertices.circle(prim.outer_diameter)
            vertices.translate(pts_o, x0, y0)
            pts_i = vertices.circle(prim.inner_diameter)
            vertices.translate(pts_i, x0, y0)
            patches.append(mpatches.Polygon(pts_o, fill=False, lw=0.5))
            patches.append(mpatches.Polygon(pts_i, fill=False, lw=0.5))
        elif isinstance(prim, gerber.MacroMoire):
            pts_o = vertices.circle(prim.outer_diameter)
            vertices.translate(pts_o, x0, y0)
            patches.append(mpatches.Polygon(pts_o, fill=False, lw=0.5))
        else:
            raise NotImplementedError('Not implemented ' + str(type(prim)))


def append_render_step_and_repeat(patches, obj):
    for i in range(obj.nx):
        for j in range(obj.ny):
            dx = i * obj.step_x * 1e6
            dy = j * obj.step_y * 1e6
            children = [copy.copy(child) for child in obj.objects]
            for child in children:
                child.translate((dx, dy))
                append_render(patches, child)


def get_poly_circle_vertices(center, radius, start_angle=0.0,
                             end_angle=90.0, max_step=10.0,
                             is_cw: bool = False):
    assert np.abs(start_angle) <= 180.0
    assert np.abs(end_angle) <= 180.0
    if end_angle == start_angle:
        end_angle = start_angle + 360.0
    if is_cw and end_angle > start_angle:
        end_angle -= 360.0
    elif not is_cw and end_angle < start_angle:
        end_angle += 360.0
    N = int(2 + np.abs(end_angle - start_angle) / max_step)
    a = np.linspace(np.deg2rad(start_angle), np.deg2rad(end_angle), N)
    x0, y0 = center
    return (x0 + radius * np.cos(a),
            y0 + radius * np.sin(a))


def test_file(filename):
    state = gerber.Gerber()
    directory = 'Gerber_File_Format_Examples 20210409'
    state.parse(os.path.join(directory, filename))
    fig, ax = plt.subplots()
    fig.suptitle(filename)
    collection = render(state)
    ax.add_collection(collection)
    ax.autoscale()
    ax.set_aspect(1)
    plt.show()


def test_hole():
    fig, ax = plt.subplots()
    xo = np.array([-2, 0, 2, 0, -2])
    yo = np.array([0, 2, 0, -2, 0])
    xi = np.array([-1, 0, 1, 0, -1])
    yi = np.array([0, -1, 0, 1, 0])
    x = np.concatenate([xo, xi])
    y = np.concatenate([yo, yi])
    xy = np.vstack([x, y]).T
    ax.add_patch(mpatches.Polygon(xy, closed=True, edgecolor='black'))
    ax.autoscale()
    ax.set_aspect(1)
    plt.show()


if __name__ == '__main__':
    # test_hole()
    test_file('2-13-1_Two_square_boxes.gbr')
    test_file('2-13-2_Polarities_and_Apertures.gbr')
    test_file('6-1-6-2_A_drill_file.gbr')
    test_file('4-6-4_Nested_blocks.gbr')
    test_file('4-11-6_Block_with_different_orientations.gbr')
    test_file('sample_macro.gbr')
    test_file('sample_macro_X1.gbr')
    test_file('SMD_prim_20.gbr')
    test_file('SMD_prim_20_X1.gbr')
    test_file('SMD_prim_21.gbr')
    test_file('SMD_prim_21_X1.gbr')
