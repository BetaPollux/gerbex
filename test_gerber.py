#!/usr/bin/python3

import os
import pytest
from pytest import approx
import gerber
from gerber import Gerber


def test_format():
    state = Gerber()
    state.set_format(r'%FSLAX36Y36*%')
    assert state.format_num_int == 3
    assert state.format_num_dec == 6


def test_format_twice():
    state = Gerber()
    state.set_format(r'%FSLAX36Y36*%')
    with pytest.raises(ValueError):
        state.set_format(r'%FSLAX36Y36*%')


def test_format_mismatch_int():
    state = Gerber()
    with pytest.raises(ValueError):
        state.set_format(r'%FSLAX36Y46*%')


def test_format_mismatch_dec():
    state = Gerber()
    with pytest.raises(ValueError):
        state.set_format(r'%FSLAX36Y35*%')


def test_load_polarity_default():
    state = Gerber()
    assert state.transform.polarity == 'dark'


def test_load_polarity_clear():
    state = Gerber()
    state.load_polarity(r'%LPC*%')
    assert state.transform.polarity == 'clear'


def test_load_polarity_dark():
    state = Gerber()
    state.load_polarity(r'%LPC*%')
    state.load_polarity(r'%LPD*%')
    assert state.transform.polarity == 'dark'


def test_load_polarity_bad():
    state = Gerber()
    with pytest.raises(ValueError):
        state.set_format(r'%LPK*%')


def test_load_mirroring_default():
    state = Gerber()
    assert state.transform.mirroring == 'N'


def test_load_mirroring_x():
    state = Gerber()
    state.load_mirroring('%LMX*%')
    assert state.transform.mirroring == 'X'


def test_load_mirroring_y():
    state = Gerber()
    state.load_mirroring('%LMY*%')
    assert state.transform.mirroring == 'Y'


def test_load_mirroring_xy():
    state = Gerber()
    state.load_mirroring('%LMXY*%')
    assert state.transform.mirroring == 'XY'


def test_load_mirroring_none():
    state = Gerber()
    state.load_mirroring('%LMX*%')
    state.load_mirroring('%LMN*%')
    assert state.transform.mirroring == 'N'


def test_load_mirroring_bad():
    state = Gerber()
    with pytest.raises(ValueError):
        state.load_mirroring('%LMXX*%')


def test_load_rotation_default():
    state = Gerber()
    assert state.transform.rotation == 0.0


def test_load_rotation():
    state = Gerber()
    state.load_rotation(r'%LR45.0*%')
    assert state.transform.rotation == 45.0


def test_load_scaling_default():
    state = Gerber()
    assert state.transform.scaling == 1.0


def test_load_scaling():
    state = Gerber()
    state.load_scaling(r'%LS0.8*%')
    assert state.transform.scaling == 0.8


def test_aperture_define():
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    assert state.apertures['D100'] is not None


def test_aperture_define_twice():
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    with pytest.raises(ValueError):
        state.aperture_define('%ADD100C,4.0*%')


def test_aperture_define_bad_ident():
    state = Gerber()
    with pytest.raises(ValueError):
        state.aperture_define('%ADC100C,1.5*%')


def test_aperture_define_bad_template():
    state = Gerber()
    with pytest.raises(KeyError):
        state.aperture_define('%ADD100Q,1.5*%')


def test_aperture_define_missing_params():
    state = Gerber()
    with pytest.raises(ValueError):
        state.aperture_define('%ADD100C*%')


def test_aperture_define_circle():
    state = Gerber()
    state.aperture_define('%ADD10C,.025*%')
    assert type(state.apertures['D10']) == gerber.Circle
    assert state.apertures['D10'].diameter == 0.025
    assert state.apertures['D10'].hole_diameter is None


def test_aperture_define_circle_hole():
    state = Gerber()
    state.aperture_define('%ADD10C,0.5X0.25*%')
    assert type(state.apertures['D10']) == gerber.Circle
    assert state.apertures['D10'].diameter == 0.5
    assert state.apertures['D10'].hole_diameter == 0.25


def test_aperture_define_rectangle():
    state = Gerber()
    state.aperture_define('%ADD22R,0.044X0.025*%')
    assert type(state.apertures['D22']) == gerber.Rectangle
    assert state.apertures['D22'].x_size == 0.044
    assert state.apertures['D22'].y_size == 0.025
    assert state.apertures['D22'].hole_diameter is None


def test_aperture_define_rectangle_hole():
    state = Gerber()
    state.aperture_define('%ADD23R,0.044X0.025X0.019*%')
    assert type(state.apertures['D23']) == gerber.Rectangle
    assert state.apertures['D23'].x_size == 0.044
    assert state.apertures['D23'].y_size == 0.025
    assert state.apertures['D23'].hole_diameter == 0.019


def test_aperture_define_obround():
    state = Gerber()
    state.aperture_define('%ADD22O,0.046X0.026*%')
    assert type(state.apertures['D22']) == gerber.Obround
    assert state.apertures['D22'].x_size == 0.046
    assert state.apertures['D22'].y_size == 0.026
    assert state.apertures['D22'].hole_diameter is None


def test_aperture_define_obround():
    state = Gerber()
    state.aperture_define('%ADD22O,0.046X0.026X0.019*%')
    assert type(state.apertures['D22']) == gerber.Obround
    assert state.apertures['D22'].x_size == 0.046
    assert state.apertures['D22'].y_size == 0.026
    assert state.apertures['D22'].hole_diameter == 0.019


def test_aperture_define_polygon():
    state = Gerber()
    state.aperture_define('%ADD17P,.040X6*%')
    assert type(state.apertures['D17']) == gerber.Polygon
    assert state.apertures['D17'].outer_diameter == 0.040
    assert state.apertures['D17'].vertices == 6
    assert state.apertures['D17'].rotation == 0.0
    assert state.apertures['D17'].hole_diameter is None


def test_aperture_define_polygon_hole():
    state = Gerber()
    state.aperture_define('%ADD17P,.040X6X0.0X0.019*%')
    assert type(state.apertures['D17']) == gerber.Polygon
    assert state.apertures['D17'].outer_diameter == 0.040
    assert state.apertures['D17'].vertices == 6
    assert state.apertures['D17'].rotation == 0.0
    assert state.apertures['D17'].hole_diameter == 0.019


def test_aperture_macro():
    state = Gerber()
    statement = '%AMCIRC*\n1,1,1.5,0,0,0*%'
    state.aperture_macro('%AMCIRC*\n1,1,1.5,0,0,0*%')
    assert type(state.templates['CIRC']) == gerber.Macro
    assert state.templates['CIRC'].template_str == '%AMCIRC*\n1,1,1.5,0,0,0*%'
    assert len(state.templates['CIRC'].primitives) == 0


def test_aperture_define_macro():
    state = Gerber()
    state.aperture_macro('%AMCIRC*\n1,1,1.5,0,0,0*%')
    state.aperture_define('%ADD15CIRC*%')
    assert type(state.apertures['D15']) == gerber.Macro
    assert state.apertures['D15'] != state.templates['CIRC']
    assert len(state.apertures['D15'].primitives) == 1
    assert type(state.apertures['D15'].primitives[0]) == gerber.MacroCircle


def test_set_current_aperture():
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    state.set_current_aperture('D100*')
    assert state.current_aperture == 'D100'


def test_set_current_aperture_undefined():
    state = Gerber()
    with pytest.raises(KeyError):
        state.set_current_aperture('D100*')


@pytest.mark.parametrize(
    "statement, x, y",
    [
        ('X2512000Y115000D01*', 2512000, 115000),
        ('X15752000D01*', 15752000, 0),
        ('Y12221000D01*', 0, 12221000),
    ],
)
def test_draw_operation(statement, x, y):
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    state.set_current_aperture('D100*')
    state.set_interpolation('G01*')
    state.move_operation('X0Y0D02*')  # Set 0, 0
    state.interpolate_operation(statement)
    assert state.current_point == (x, y)
    assert type(state.objects[0]) == gerber.Draw
    assert state.objects[0].endpoint == (x, y)


@pytest.mark.parametrize(
    "statement, x, y, i, j",
    [
        ('X5005000Y3506000I3000J0D01*', 5005000, 3506000, 3000, 0),
        ('X30000000Y25000000I0J-3750000D01*', 30000000, 25000000, 0, -3750000),
        ('X-3000000Y-2000000I-3000000J4000000D01*', -3000000, -2000000, -3000000, 4000000)
    ],
)
def test_arc_operation(statement, x, y, i, j):
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    state.set_current_aperture('D100*')
    state.set_interpolation('G02*')
    state.move_operation('X0Y0D02*')  # Set 0, 0
    state.interpolate_operation(statement)
    assert state.current_point == (x, y)
    assert type(state.objects[0]) == gerber.Arc
    assert state.objects[0].endpoint == (x, y)
    assert state.objects[0].offset == (i, j)


@pytest.mark.parametrize(
    "statement, x, y",
    [
        ('X300Y-400D03*', 300, -400),
        ('Y-2000000D03*', 0, -2000000)
    ],
)
def test_flash_operation(statement, x, y):
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    state.set_current_aperture('D100*')
    state.move_operation('X0Y0D02*')  # Set 0, 0
    state.flash_operation(statement)
    assert state.current_point == (x, y)
    assert type(state.objects[0]) == gerber.Flash


@pytest.mark.parametrize(
    "statement, x, y",
    [
        ('X0Y0D02*', 0, 0),
        ('X2152000Y1215000D02*', 2152000, 1215000),
        ('X-10000D02*', -10000, 0)
    ],
)
def test_move_operation(statement, x, y):
    state = Gerber()
    state.move_operation('X0Y0D02*')  # Set 0, 0
    state.move_operation(statement)
    assert state.current_point == (x, y)
    assert len(state.objects) == 0


def test_flash_operation_polarity():
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    state.set_current_aperture('D100*')
    state.flash_operation('X2152000Y1215000D03*')
    state.load_polarity('%LPC*%')
    state.flash_operation('X3152000Y2215000D03*')
    assert state.objects[0].origin == (2152000, 1215000)
    assert state.objects[0].transform.polarity == 'dark'
    assert state.objects[1].origin == (3152000, 2215000)
    assert state.objects[1].transform.polarity == 'clear'


def test_draw_operation_polarity():
    state = Gerber()
    state.aperture_define('%ADD100C,1.5*%')
    state.set_current_aperture('D100*')
    state.set_interpolation('G01*')
    state.move_operation('X1512000Y155000D02*')
    state.interpolate_operation('X2512000Y115000D01*')
    state.load_polarity('%LPC*%')
    state.interpolate_operation('X2712000Y117000D01*')
    assert state.objects[0].origin == (1512000, 155000)
    assert state.objects[0].endpoint == (2512000, 115000)
    assert state.objects[0].transform.polarity == 'dark'
    assert state.objects[1].origin == (2512000, 115000)
    assert state.objects[1].endpoint == (2712000, 117000)
    assert state.objects[1].transform.polarity == 'clear'


def test_region_triangle():
    state = Gerber()
    state.begin_region('G36*')
    state.move_operation('X100000Y100000D02*')
    state.set_interpolation('G01*')
    state.interpolate_operation('X500000Y100000D01*')
    state.interpolate_operation('X300000Y400000D01*')
    state.interpolate_operation('X100000Y100000D01*')
    state.end_region('G37*')
    assert type(state.objects[0]) == gerber.Region
    assert len(state.objects[0].objects) == 3
    assert state.objects[0].objects[0].origin == (100000, 100000)
    assert state.objects[0].objects[0].endpoint == (500000, 100000)
    assert state.objects[0].objects[1].origin == (500000, 100000)
    assert state.objects[0].objects[1].endpoint == (300000, 400000)
    assert state.objects[0].objects[2].origin == (300000, 400000)
    assert state.objects[0].objects[2].endpoint == (100000, 100000)
    assert state.objects[0].contours == [(0, 3)]


def test_region_triangle_and_square():
    state = Gerber()
    state.begin_region('G36*')
    state.move_operation('X100000Y100000D02*')
    state.set_interpolation('G01*')
    state.interpolate_operation('X500000Y100000D01*')
    state.interpolate_operation('X300000Y400000D01*')
    state.interpolate_operation('X100000Y100000D01*')
    state.move_operation('X0Y200000D02*')
    state.interpolate_operation('X0Y300000D01*')
    state.interpolate_operation('X-100000Y300000D01*')
    state.interpolate_operation('X-100000Y200000D01*')
    state.interpolate_operation('X0Y200000D01*')
    state.end_region('G37*')
    assert type(state.objects[0]) == gerber.Region
    assert len(state.objects[0].objects) == 7
    assert state.objects[0].objects[0].origin == (100000, 100000)
    assert state.objects[0].objects[0].endpoint == (500000, 100000)
    assert state.objects[0].objects[1].origin == (500000, 100000)
    assert state.objects[0].objects[1].endpoint == (300000, 400000)
    assert state.objects[0].objects[2].origin == (300000, 400000)
    assert state.objects[0].objects[2].endpoint == (100000, 100000)
    assert state.objects[0].contours == [(0, 3), (3, 4)]


def test_macro_circle():
    primitive = gerber.MacroCircle.parse('1,1,1.5,0.125,0.375,45.0')
    assert primitive.exposure == 1
    assert primitive.diameter == 1.5
    assert primitive.x == 0.125
    assert primitive.y == 0.375
    assert primitive.rotation == 45.0


def test_macro_circle_no_rotation():
    primitive = gerber.MacroCircle.parse('1,1,1.5,0.125,0.375')
    assert primitive.exposure == 1
    assert primitive.diameter == 1.5
    assert primitive.x == 0.125
    assert primitive.y == 0.375
    assert primitive.rotation == 0.0


def test_macro_vector_line():
    primitive = gerber.MacroVectorLine.parse('20,1,0.9,0,0.45,12,0.45,0')
    assert primitive.exposure == 1
    assert primitive.width == 0.9
    assert primitive.x1 == 0
    assert primitive.y1 == 0.45
    assert primitive.x2 == 12
    assert primitive.y2 == 0.45
    assert primitive.rotation == 0


def test_macro_center_line():
    primitive = gerber.MacroCenterLine.parse('21,1,6.8,1.2,3.4,0.6,30')
    assert primitive.exposure == 1
    assert primitive.width == 6.8
    assert primitive.height == 1.2
    assert primitive.x == 3.4
    assert primitive.y == 0.6
    assert primitive.rotation == 30


def test_macro_polygon():
    primitive = gerber.MacroPolygon.parse('5,1,8,0.5,0.2,4,30')
    assert primitive.exposure == 1
    assert primitive.vertices == 8
    assert primitive.x == 0.5
    assert primitive.y == 0.2
    assert primitive.diameter == 4
    assert primitive.rotation == 30


def test_macro_thermal():
    primitive = gerber.MacroThermal.parse('7,0.1,0.2,0.800,0.550,0.125,45')
    assert primitive.x == 0.1
    assert primitive.y == 0.2
    assert primitive.outer_diameter == 0.8
    assert primitive.inner_diameter == 0.55
    assert primitive.gap == 0.125
    assert primitive.rotation == 45


def test_macro_moire():
    primitive = gerber.MacroMoire.parse('6,0.1,0.2,1.25,.1,0.11,3,0.03,1.50,0')
    assert primitive.x == 0.1
    assert primitive.y == 0.2
    assert primitive.outer_diameter == 1.25
    assert primitive.ring_thickness == 0.1
    assert primitive.gap == 0.11
    assert primitive.num_rings == 3
    assert primitive.crosshair_thickness == 0.03
    assert primitive.crosshair_length == 1.5
    assert primitive.rotation == 0


def test_macro_outline_three():
    primitive = gerber.MacroOutline.parse('4,1,3,1,-1,1,1,2,1,1,-1,30')
    assert primitive.exposure == 1
    assert primitive.vertices == 3
    assert primitive.x == 1
    assert primitive.y == -1
    assert primitive.coordinates == [1, 1, 2, 1, 1, -1]
    assert primitive.rotation == 30


def test_macro_outline_too_few_params():
    with pytest.raises(ValueError):
        gerber.MacroOutline.parse('4,1,3,1,-1,1,1,2,1,30')


@pytest.mark.parametrize(
    "primitive",
    [
        '1,1,1.5,0.125,0.375,45.0',
        '1,1,1.5,0.125,0.375',
        '20,1,0.9,0,0.45,12,0.45,0',
        '21,1,6.8,1.2,3.4,0.6,30',
        '5,1,8,0.5,0.2,4,30',
        '7,0.1,0.2,0.800,0.550,0.125,45',
        '6,0.1,0.2,1.25,.1,0.11,3,0.03,1.50,0',
        '4,1,3,1,-1,1,1,2,1,1,-1,30'
    ],
)
def test_macro_primitive_creation(primitive):
    state = Gerber()
    state.aperture_macro('%AMSINGLE*' + primitive + '*%')
    state.aperture_define('%ADD34SINGLE*%')
    assert 'SINGLE' in state.templates
    assert 'D34' in state.apertures


@pytest.mark.parametrize(
    "bad_code",
    [
        '2',
        '22',
        '9'
    ],
)
def test_macro_invalid_primitive_creation(bad_code):
    state = Gerber()
    with pytest.raises(KeyError):
        state.aperture_macro(f'%AMSINGLE*{bad_code},1,2,3*%')
        state.aperture_define('%ADD34SINGLE*%')


def test_macro_ignores_comments():
    state = Gerber()
    state.aperture_macro('%AMSINGLE*\n0 A comment*\n1,1,1.5,0.125,0.375*%')
    state.aperture_define('%ADD34SINGLE*%')
    assert 'SINGLE' in state.templates
    assert 'D34' in state.apertures


def test_block_aperture():
    state = Gerber()
    state.aperture_define('%ADD10C,7.500000*%')
    state.aperture_define('%ADD11C,15*%')
    # G04 Define block aperture D100, consisting of two draws and a round dot*
    state.aperture_block('%ABD100*%')
    state.set_current_aperture('D10*')
    state.move_operation('X65532000Y17605375D02*')
    state.interpolate_operation('Y65865375D01*')
    state.interpolate_operation('X-3556000D01*')
    state.set_current_aperture('D11*')
    state.flash_operation('X-3556000Y17605375D03*')
    state.aperture_block('%AB*%')
    assert len(state.objects) == 0
    block = state.apertures['D100']
    assert type(block) == gerber.BlockAperture
    assert type(block.objects[0]) == gerber.Draw
    assert type(block.objects[1]) == gerber.Draw
    assert type(block.objects[2]) == gerber.Flash
    assert block.objects[0].aperture.diameter == 7.5
    assert block.objects[1].aperture.diameter == 7.5
    assert block.objects[2].aperture.diameter == 15


def test_nested_block():
    state = Gerber()
    state.aperture_define('%ADD12R,20X10*%')
    # G04 Define block aperture  D102, consisting of 2x3 flashes of D101*
    state.aperture_block('%ABD102*%')
    # G04 Define nested block aperture D101, consisting of 2x2 flashes of D12*
    state.aperture_block('%ABD101*%')
    state.set_current_aperture('D12*')
    state.flash_operation('X0Y0D03*')
    state.flash_operation('X0Y70000000D03*')
    state.flash_operation('X100000000Y0D03*')
    state.flash_operation('X100000000Y70000000D03*')
    state.aperture_block('%AB*%')
    state.set_current_aperture('D101*')
    state.flash_operation('X0Y0D03*')
    state.flash_operation('X0Y160000000D03*')
    state.flash_operation('X0Y320000000D03*')
    state.flash_operation('X230000000Y0D03*')
    state.flash_operation('X230000000Y160000000D03*')
    state.flash_operation('X230000000Y320000000D03*')
    state.aperture_block('%AB*%')
    assert len(state.objects) == 0
    parent = state.apertures['D102']
    child = state.apertures['D101']
    assert len(parent.objects) == 6
    assert len(child.objects) == 4
    for parent_obj in parent.objects:
        assert type(parent_obj.aperture) == gerber.BlockAperture
    for child_obj in child.objects:
        assert type(child_obj.aperture) == gerber.Rectangle


def test_macro_variables():
    state = Gerber()
    state.aperture_macro('%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%')
    state.aperture_define('%ADD34DONUTVAR,0.100X0X0X0.080*%')
    assert type(state.templates['DONUTVAR']) == gerber.Macro
    assert type(state.apertures['D34']) == gerber.Macro
    assert len(state.apertures['D34'].primitives) == 2
    assert state.apertures['D34'].primitives[0].exposure == 1
    assert state.apertures['D34'].primitives[0].diameter == 0.1
    assert state.apertures['D34'].primitives[0].x == 0
    assert state.apertures['D34'].primitives[0].y == 0
    assert state.apertures['D34'].primitives[1].exposure == 0
    assert state.apertures['D34'].primitives[1].diameter == 0.08
    assert state.apertures['D34'].primitives[1].x == 0
    assert state.apertures['D34'].primitives[1].y == 0


def test_macro_missing_variable():
    state = Gerber()
    with pytest.raises(IndexError):
        state.aperture_macro('%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%')
        state.aperture_define('%ADD34DONUTVAR,0.100X0X0*%')


@pytest.mark.parametrize(
    "expr, result",
    [
        ('5', 5),
        ('1.25', 1.25),
        ('+3.125', 3.125),
        ('-2.0', -2.0),
        ('0.75+0.25', 1.0),
        ('2x1.25', 2.50),
        ('2.50/5', 0.5),
        ('-5.0+3.0', -2.0),
        ('3.0-3x3.0', -6.0),
        ('1.25x2-3', -0.5),
        ('1.5x0.5', 0.75),
        ('(1.25-1.0)x2', 0.5)
    ],
)
def test_macro_expressions(expr, result):
    assert gerber.Macro.eval_expression(expr) == approx(result)


@pytest.mark.parametrize(
    "expr",
    [
        '1.25X3',
        '1.25*2',
        'a*b',
        '$1/2',
        '1|2',
        'import sys'
    ],
)
def test_macro_expressions_illegal(expr):
    with pytest.raises(ValueError):
        gerber.Macro.eval_expression(expr)


@pytest.mark.parametrize(
    "filename",
    [
        '2-13-1_Two_square_boxes.gbr',
        '2-13-1_Two_square_boxes.gbr',
        '2-13-2_Polarities_and_Apertures.gbr',
        '4-11-6_Block_with_different_orientations.gbr',
        '4-6-4_Nested_blocks.gbr',
        '6-1-6-2_A_drill_file.gbr',
        'sample_macro.gbr',
        'sample_macro_X1.gbr',
        'SMD_prim_20.gbr',
        'SMD_prim_20_X1.gbr',
        'SMD_prim_21.gbr',
        'SMD_prim_21_X1.gbr'
    ],
)
def test_parse(filename):
    state = Gerber()
    directory = 'Gerber_File_Format_Examples 20210409'
    state.parse(os.path.join(directory, filename))
