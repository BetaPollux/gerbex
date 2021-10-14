#!/usr/bin/python3

# Plot Gerber object stream using matplotlib

import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib.path as mpath
import matplotlib as mpl
import gerber
import vertices
import copy

# TODO remove magic 1e6 and fix units/scaling
# TODO move vertex calcs into aperture classes etc
# TODO render holes
# TODO render transforms
# TODO add tests


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
    if type(obj.aperture) in (gerber.Circle,
                              gerber.Rectangle,
                              gerber.Obround,
                              gerber.Polygon):
        outline = obj.get_outline()  # TODO apply scale factor
        patches.append(get_path_patch(outline))
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
        polys = []
        poly_kwargs = {}
        if isinstance(prim, gerber.MacroCircle):
            pts = vertices.circle(prim.diameter)
            polys.append(pts)
        elif isinstance(prim, gerber.MacroVectorLine):
            pts = vertices.thick_line(prim.width,
                                      prim.x1, prim.y1,
                                      prim.x2, prim.y2)
            polys.append(pts)
        elif isinstance(prim, gerber.MacroCenterLine):
            pts = vertices.rectangle(prim.width, prim.height)
            polys.append(pts)
        elif isinstance(prim, gerber.MacroOutline):
            N = int(len(prim.coordinates) / 2)
            pts = np.array(prim.coordinates)
            pts.resize((N, 2))
            # TODO Outline does not need translate at all
            # This compensates for common translate later
            vertices.translate(pts, -prim.x, -prim.y)
            polys.append(pts)
        elif isinstance(prim, gerber.MacroPolygon):
            pts = vertices.regular_poly(prim.diameter, prim.vertices)
            polys.append(pts)
        elif isinstance(prim, gerber.MacroThermal):
            elements = [
                vertices.circle(prim.outer_diameter),
                vertices.circle(prim.inner_diameter),
                vertices.rectangle(prim.outer_diameter, prim.gap),
                vertices.rectangle(prim.gap, prim.outer_diameter)]
            for pts in elements:
                polys.append(pts)
            poly_kwargs['fill'] = False
            poly_kwargs['lw'] = 0.5
        elif isinstance(prim, gerber.MacroMoire):
            pts = vertices.circle(prim.outer_diameter)
            polys.append(pts)
            poly_kwargs['fill'] = False
        else:
            raise NotImplementedError('Not implemented ' + str(type(prim)))

        if prim.exposure == 0:
            poly_kwargs['fill'] = False
            poly_kwargs['lw'] = 0.5
            poly_kwargs['ls'] = 'dotted'
        for pts in polys:
            vertices.translate(pts, prim.x, prim.y)
            vertices.rotate(pts, prim.rotation)
            vertices.translate(pts, x0, y0)
            patches.append(mpatches.Polygon(pts, **poly_kwargs))


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


def get_path_patch(outline: vertices.OutlineVertices):
    def pad(pts):
        """Append dummy vertex, to allow CLOSEPOLY."""
        return np.vstack([pts, [[0, 0]]])

    def get_codes(pts):
        """Assumes pts has an added dummy vertex."""
        return [mpath.Path.MOVETO] + \
               [mpath.Path.LINETO] * (len(pts) - 1) + \
               [mpath.Path.CLOSEPOLY]
    all_vertices = np.vstack([pad(outline.boundary),
                              *[pad(hole) for hole in outline.holes]])
    codes = get_codes(outline.boundary)
    if outline.holes:
        codes.extend(*[get_codes(hole) for hole in outline.holes])
    path = mpath.Path(all_vertices, codes)
    color = 'blue' if outline.positive else 'white'
    return mpatches.PathPatch(path, color=color)


def test_path_patch():
    donut = gerber.Circle(1.5, 0.5)
    plate = gerber.Circle(0.2)
    rect = gerber.Rectangle(0.5, 1.2, 0.2)
    outline1 = donut.get_outline()
    outline2 = plate.get_outline()
    outline3 = rect.get_outline()
    outline3.translate(3.0, 0.0)
    pp1 = get_path_patch(outline1)
    pp2 = get_path_patch(outline2)
    pp3 = get_path_patch(outline3)
    ax = plt.gca()
    ax.add_patch(pp1)
    ax.add_patch(pp2)
    ax.add_patch(pp3)
    ax.autoscale()
    plt.show()


if __name__ == '__main__':
    test_path_patch()
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
