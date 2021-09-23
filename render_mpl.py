#!/usr/bin/python3

# Plot Gerber object stream using matplotlib

import os
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import matplotlib as mpl
import gerber
import copy

# TODO move vertex calcs into aperture classes etc
# TODO render holes
# TODO render transforms
# TODO add tests


def render(state):
    patches = []
    for obj in state.objects:
        render = get_render(obj)
        if type(render) == list:
            # TODO this needs to handle nested lists
            patches.extend(render)
        else:
            patches.append(render)
    return mpl.collections.PatchCollection(patches, match_original=True)


def get_render(obj):
    if isinstance(obj, gerber.Draw):
        return render_draw(obj)
    elif isinstance(obj, gerber.Flash):
        return render_flash(obj)
    elif isinstance(obj, gerber.Arc):
        return render_arc(obj)
    elif isinstance(obj, gerber.Region):
        return render_region(obj)
    else:
        raise TypeError('Unsupported object type for render')


def render_draw(obj):
    assert isinstance(obj, gerber.Draw)
    assert isinstance(obj.aperture, gerber.Circle)
    x0, y0 = obj.origin
    x1, y1 = obj.endpoint
    # TODO should have rounded ends
    if x1 != x0:
        angle = np.arctan2(y1 - y0, x1 - x0)
    else:
        angle = 0.5 * np.pi
    r = 1e6 * 0.5 * obj.aperture.diameter
    dx = r * np.sin(angle)
    dy = r * np.cos(angle)
    return mpatches.Polygon(1e-6 * np.array([[x0 - dx, x1 - dx, x1 + dx, x0 + dx], [y0 + dy, y1 + dy, y1 - dy, y0 - dy]]).T)


def render_arc(obj):
    assert isinstance(obj, gerber.Arc)
    assert isinstance(obj.aperture, gerber.Circle)
    dx, dy = obj.offset
    x1, y1 = obj.origin
    x0, y0 = x1 + dx, y1 + dy
    x2, y2 = obj.endpoint
    r = np.sqrt((x1-x0)**2 + (y1-y0)**2)
    dr = 1e6 * 0.5 * obj.aperture.diameter
    start_angle = np.rad2deg(np.arctan2(y1-y0, x1-x0))
    end_angle = np.rad2deg(np.arctan2(y2-y0, x2-x0))
    # TODO should have rounded ends
    xco, yco = get_poly_circle_vertices((x0, y0), r+dr, start_angle, end_angle, is_cw=obj.is_cw)
    xci, yci = get_poly_circle_vertices((x0, y0), r-dr, start_angle, end_angle, is_cw=obj.is_cw)
    xci, yci = np.flip(xci), np.flip(yci)
    xc = np.concatenate([xco, xci])
    yc = np.concatenate([yco, yci])
    xy = 1e-6 * np.vstack([xc, yc]).T
    return mpatches.Polygon(xy, fill=False)


def render_region(obj):
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
            xc, yc = get_poly_circle_vertices((x0, y0), r, start_angle, end_angle, is_cw=segment.is_cw)
            x.extend(xc)
            y.extend(yc)
        else:
            raise TypeError('Segment not supported')
    xy = 1e-6 * np.vstack([x, y]).T
    return mpatches.Polygon(xy, fill=False)


def render_flash(obj):
    assert isinstance(obj, gerber.Flash)
    assert obj.aperture is not None
    if isinstance(obj.aperture, gerber.Circle):
        return mpatches.CirclePolygon(1e-6 * np.array(obj.origin),
                                      0.5 * obj.aperture.diameter,
                                      resolution=36)
    elif isinstance(obj.aperture, gerber.Polygon):
        return mpatches.RegularPolygon(1e-6 * np.array(obj.origin),
                                       obj.aperture.vertices,
                                       0.5 * obj.aperture.outer_diameter,
                                       orientation=-0.5 * np.pi)
    elif isinstance(obj.aperture, gerber.Rectangle):
        dx = 0.5 * obj.aperture.x_size
        dy = 0.5 * obj.aperture.y_size
        return mpatches.Rectangle(1e-6 * np.array(obj.origin) + np.array([-dx, -dy]),
                                  obj.aperture.x_size,
                                  obj.aperture.y_size)
    elif isinstance(obj.aperture, gerber.Obround):
        x0, y0 = 1e-6 * np.array(obj.origin)
        dx = 0.5 * obj.aperture.x_size
        dy = 0.5 * obj.aperture.y_size
        # TODO make circle ends not triangles
        if obj.aperture.x_size > obj.aperture.y_size:
            r = 0.5 * obj.aperture.y_size
            x = np.array([x0 + dx, x0 + dx - r, x0 - dx + r, x0 - dx, x0 - dx + r, x0 + dx - r])
            y = np.array([y0, y0 + dy, y0 + dy, y0, y0 - dy, y0 - dy])
        else:
            r = 0.5 * obj.aperture.x_size
            x = np.array([x0, x0 + dx, x0 + dx, x0, x0 - dx, x0 - dx])
            y = np.array([y0 + dy, y0 + dy - r, y0 - dy + r, y0 - dy, y0 - dy + r, y0 + dy - r])
        xy = np.vstack([x, y]).T
        return mpatches.Polygon(xy)
    elif isinstance(obj.aperture, gerber.Macro):
        # TODO render macro primitives
        return mpatches.CirclePolygon(1e-6 * np.array(obj.origin),
                                      0.2, color='r',
                                      resolution=10)
    elif isinstance(obj.aperture, gerber.BlockAperture):
        # TODO render nested blocks
        block_children = [copy.copy(block) for block in obj.aperture.objects]
        for child in block_children:
            child.translate(obj.origin)
        return [get_render(block) for block in block_children]
    else:
        raise NotImplementedError('Unrecognized Aperture ' + str(type(obj.aperture)))


def get_poly_circle_vertices(center, radius, start_angle=0.0, end_angle=90.0, max_step=10.0, is_cw: bool = False):
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


def test_one():
    obj = gerber.Draw(None, None, (0, 0), (1, 1))
    fig, ax = plt.subplots()
    ax.fill(*get_poly_circle_vertices((1.0, 0), 0.5, 180.0, -180.0))
    ax.plot(*get_poly_circle_vertices((0.0, 1.5), 0.3, 45.0, 180.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((0.0, 1.5), 0.3, 45.0, 180.0, is_cw=False), ls='dashed')
    ax.plot(*get_poly_circle_vertices((1.0, 1.5), 0.3, -30.0, -150.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((1.0, 1.5), 0.3, -30.0, -150.0, is_cw=False), ls='dashed')
    ax.plot(*get_poly_circle_vertices((2.0, 1.5), 0.3, -30.0, 150.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((2.0, 1.5), 0.3, -30.0, 150.0, is_cw=False), ls='dashed')
    ax.plot(*get_poly_circle_vertices((3.0, 1.5), 0.3, 30.0, -150.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((3.0, 1.5), 0.3, 30.0, -150.0, is_cw=False), ls='dashed')
    ax.plot(*get_poly_circle_vertices((0.0, 2.5), 0.3, 180.0, 45.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((0.0, 2.5), 0.3, 180.0, 45.0, is_cw=False), ls='dashed')
    ax.plot(*get_poly_circle_vertices((1.0, 2.5), 0.3, -150.0, -30.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((1.0, 2.5), 0.3, -150.0, -30.0, is_cw=False), ls='dashed')
    ax.plot(*get_poly_circle_vertices((2.0, 2.5), 0.3, -150.0, 30.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((2.0, 2.5), 0.3, -150.0, 30.0, is_cw=False), ls='dashed')
    ax.plot(*get_poly_circle_vertices((3.0, 2.5), 0.3, -150.0, -30.0, is_cw=True))
    ax.plot(*get_poly_circle_vertices((3.0, 2.5), 0.3, -150.0, -30.0, is_cw=False), ls='dashed')
    flash = gerber.Flash(gerber.Circle(0.25), None, (0, 0))
    ax.add_patch(render_flash(flash))
    ax.set_aspect(1)
    plt.show()


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
    test_one()
    # test_hole()
    test_file('2-13-1_Two_square_boxes.gbr')
    test_file('2-13-2_Polarities_and_Apertures.gbr')
    test_file('6-1-6-2_A_drill_file.gbr')
    # test_file('4-6-4_Nested_blocks.gbr')
    test_file('4-11-6_Block_with_different_orientations.gbr')
    # test_file('sample_macro.gbr')
    # test_file('sample_macro_X1.gbr')
    # test_file('SMD_prim_20.gbr')
