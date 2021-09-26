#!/usr/bin/python3

# RS-274X per standard Revision 2021.02

import re
import copy

# Meant for extracting substrings only
# Cast to int or float will catch invalid strings
RE_INT = r'[+-]?[0-9]+'
RE_DEC = r'[+-]?[0-9\.]+?'


class Gerber():
    def __init__(self):
        self.format_num_int = None
        self.format_num_dec = None
        self.unit = None
        self.current_point = None
        self.current_aperture = None
        # Interpolation should be None, but not all files have G01
        self.interpolation = 'linear'
        self.region = None
        self.transform = ApertureTransform()
        self.apertures = {}
        self.templates = {
            'C': Circle(1.0),
            'R': Rectangle(1.0, 1.0),
            'O': Obround(1.0, 1.0),
            'P': Polygon(1.0, 3, 0.0)
        }
        self.objects = []
        self.objects_list_stack = [self.objects]
        self.reached_eof = False

    def add_object(self, new_obj):
        self.objects_list_stack[-1].append(new_obj)

    def comment(self, statement: str):
        pass

    def ignore(self, statement: str):
        pass

    def not_implemented(self, statement: str):
        raise NotImplementedError('Command not implemented: ' + statement)

    def begin_region(self, statement: str):
        # TODO is self.region required?
        self.region = Region(self.transform)
        self.objects_list_stack.append(self.region)

    def end_region(self, statement: str):
        self.region.end_contour()
        self.objects_list_stack.pop()
        self.add_object(self.region)
        self.region = None

    def get_command_function(self, statement: str):
        commands = {
            'G04': self.comment,
            'MO': self.set_mode,
            'FS': self.set_format,
            'AD': self.aperture_define,
            'AM': self.aperture_macro,
            'Dnn': self.set_current_aperture,
            'D01': self.interpolate_operation,
            'D02': self.move_operation,
            'D03': self.flash_operation,
            'G01': self.set_interpolation,
            'G02': self.set_interpolation,
            'G03': self.set_interpolation,
            'G74': self.not_implemented,
            'G75': self.ignore,
            'LP': self.load_polarity,
            'LM': self.load_mirroring,
            'LR': self.load_rotation,
            'LS': self.load_scaling,
            'G36': self.begin_region,
            'G37': self.end_region,
            'AB': self.aperture_block,
            'SR': self.step_and_repeat,
            'TF': self.ignore,
            'TA': self.ignore,
            'TO': self.ignore,
            'TD': self.ignore,
            'M02': self.set_eof
        }
        # Extended commands
        # e.g.
        #   %MOMM*%
        #   %AMDonut*
        #   1,1,$1,$2,$3*
        #   $4=$1x0.75*
        #   1,0,$4,$2,$3*
        #   %
        #   %ADD11Donut,0.30X0X0*%
        code = None
        if statement.startswith('%'):
            code = statement[1:3]
        else:
            # Word commands
            # e.g.
            #   G04 comment *
            #   D10*
            #   X0Y0D02*
            match = re.search(r'[GDM](\d\d)', statement)
            if match:
                code = match.group()
                if code[0] == 'D' and int(match.group(1)) >= 10:
                    code = 'Dnn'
        try:
            return commands[code]
        except KeyError:
            raise KeyError(f'Unrecognized statement: {statement}')

    def set_eof(self, statement: str):
        if statement != 'M02*':
            raise ValueError('Invalid EOF statement')
        self.reached_eof = True

    def parse(self, filename: str):
        with open(filename, 'r') as f:
            delimiter = False
            for line_num, line in enumerate(f):
                if line.isspace():
                    continue

                if line.startswith('%'):
                    delimiter = True
                    statement = ''

                if delimiter:
                    statement += line
                else:
                    statement = line

                if line.endswith('%\n'):
                    delimiter = False

                if not delimiter:
                    statement = statement.strip()
                    try:
                        command = self.get_command_function(statement)
                        command(statement)
                    except (ValueError, KeyError) as ex:
                        raise ValueError(f'Error line {line_num + 1}: {ex}')
        if not self.reached_eof:
            raise ValueError('File did not contain EOF marker')

    def set_mode(self, statement: str):
        # Set unit of measurement to metric or imperial
        if statement == '%MOMM*%':
            self.unit = 'mm'
        elif statement == '%MOIN*%':
            self.unit = 'in'
        else:
            raise ValueError(f'Unrecognized mode statement: {statement}')

    def set_format(self, statement: str):
        # Set coordinates and distances in operations
        # %FSLAX36Y36*% sets 3 integer digits, 6 decimal digits
        # 6 decimal digits implies 10^-6 is increment
        if self.format_num_dec is not None or self.format_num_int is not None:
            raise ValueError('Format must be set exactly once')
        match = re.search(r'%FSLAX([1-6])([3-6])Y([1-6])([3-6])\*%', statement)
        if match is None:
            raise ValueError(f'Unrecognized format statement: {statement}')
        if match.group(1) != match.group(3):
            raise ValueError(f'Mismatched format X, Y integer digits: {statement}')
        if match.group(2) != match.group(4):
            raise ValueError(f'Mismatched format X, Y decimal digits: {statement}')
        self.format_num_int = int(match.group(1))
        self.format_num_dec = int(match.group(2))

    def set_interpolation(self, statement: str):
        # Set interpolation state to linear or circular
        if statement == 'G01*':
            self.interpolation = 'linear'
        elif statement == 'G02*':
            self.interpolation = 'cw_circular'
        elif statement == 'G03*':
            self.interpolation = 'ccw_circular'
        else:
            raise ValueError(f'Unrecognized interpolation statement: {statement}')

    def create_aperture(self):
        if self.current_aperture is not None:
            return self.apertures[self.current_aperture].clone()
        else:
            return None

    def get_new_point(self, x, y):
        if x and y:
            return (int(x[1:]), int(y[1:]))
        elif x:
            return (int(x[1:]), self.current_point[1])
        elif y:
            return (self.current_point[0], int(y[1:]))
        else:
            raise ValueError('Invalid x and y')

    def interpolate_operation(self, statement: str):
        # D01 linear/circular line segment
        match = re.search(rf'(X{RE_INT})?(Y{RE_INT})?(I{RE_INT})?(J{RE_INT})?D01\*', statement)
        if match is not None:
            x = match.group(1)
            y = match.group(2)
            i = match.group(3)
            j = match.group(4)
            new_point = self.get_new_point(x, y)
            if self.interpolation == 'linear':
                self.add_object(Draw(self.create_aperture(), self.transform,
                                     self.current_point, new_point))
            elif self.interpolation in ('cw_circular', 'ccw_circular'):
                if i and j:
                    offset = (int(i[1:]), int(j[1:]))
                else:
                    raise ValueError(f'Missing offset: I {i}, J {j}')
                is_cw = (self.interpolation == 'cw_circular')
                self.add_object(Arc(self.create_aperture(), self.transform,
                                    self.current_point, new_point,
                                    offset, is_cw))
            else:
                raise ValueError(f'Invalid interpolation: {self.interpolation}')
            self.current_point = new_point
        else:
            raise ValueError(f'Unrecognized interpolate operation: {statement}')

    def move_operation(self, statement: str):
        # D02 move operation
        match = re.search(rf'(X{RE_INT})?(Y{RE_INT})?D02\*', statement)
        if match is not None:
            x = match.group(1)
            y = match.group(2)
            self.current_point = self.get_new_point(x, y)
        else:
            raise ValueError(f'Unrecognized move operation: {statement}')

    def flash_operation(self, statement: str):
        # D03 create flash object
        match = re.search(rf'(X{RE_INT})?(Y{RE_INT})?D03\*', statement)
        if match is not None:
            x = match.group(1)
            y = match.group(2)
            new_point = self.get_new_point(x, y)
            aperture = self.create_aperture()
            self.add_object(Flash(aperture, self.transform, new_point))
            self.current_point = new_point
        else:
            raise ValueError(f'Unrecognized flash operation: {statement}')

    def load_polarity(self, statement: str):
        # Polarity is either clear or dark
        if statement == '%LPC*%':
            self.transform.polarity = 'clear'
        elif statement == '%LPD*%':
            self.transform.polarity = 'dark'
        else:
            raise ValueError(f'Unrecognized polarity statement: {statement}')

    def load_mirroring(self, statement: str):
        # Mirror either N, X, Y or XY
        match = re.search(r'%LM(N|X|Y|XY)\*%', statement)
        if match is not None:
            self.transform.mirroring = match.group(1)
        else:
            raise ValueError(f'Unrecognized mirroring statement: {statement}')

    def load_rotation(self, statement: str):
        # Rotation in degrees counterclockwise
        match = re.search(r'%LR(\S+)\*%', statement)
        if match is not None:
            self.transform.rotation = float(match.group(1))
        else:
            raise ValueError(f'Unrecognized rotation statement: {statement}')

    def load_scaling(self, statement: str):
        # Scaling where 1.0 is no scaling
        match = re.search(r'%LS(\S+)\*%', statement)
        if match is not None:
            self.transform.scaling = float(match.group(1))
        else:
            raise ValueError(f'Unrecognized scaling statement: {statement}')

    def aperture_define(self, statement: str):
        # Parse e.g. %ADD100C,1.5*%
        # AD, D100, C, 1.5
        # cmd, ident, template
        match = re.search(r'%AD(D[0-9]{2,})([\w\.\$]+)(,\S*)?\*%', statement)
        if match is not None:
            ident = match.group(1)
            template_name = match.group(2)
            parameters = match.group(3)
            if parameters:
                parameters = parameters.lstrip(',')

            if ident in self.apertures:
                raise ValueError(f'Aperture {ident} already defined')

            if template_name in self.templates:
                self.apertures[ident] = self.templates[template_name].derive_from(parameters)
            else:
                raise KeyError(f'Aperture template {template_name} not defined')
        else:
            raise ValueError(f'Unrecognized aperture define statement: {statement}')

    def aperture_macro(self, statement: str):
        # %AMCIRC*\n1,1,1.5,0,0,0*%
        match = re.search(r'%AM([\w\.\$]+)', statement)
        if match is not None:
            ident = match.group(1)
            if ident in self.templates:
                raise ValueError(f'Aperture {ident} template already defined')
            self.templates[ident] = Macro.parse(statement)
        else:
            raise ValueError(f'Unrecognized aperture macro statement: {statement}')

    def aperture_block(self, statement: str):
        # %ABD12*%
        # %ADD11C,0.5*%
        # D10*
        # G01*
        # X-2500000Y-1000000D03*
        # Y1000000D03*
        # %LPC*%
        # ...
        # G01*
        # %AB*%
        match = re.search(r'%AB(D[0-9]{2,})?\*%', statement)
        if match is not None:
            ident = match.group(1)
            if ident is None:  # Close Block
                self.objects_list_stack.pop()
            else:  # Open new Block
                if ident in self.apertures:
                    raise ValueError(f'Aperture {ident} already defined')
                self.apertures[ident] = BlockAperture()
                self.objects_list_stack.append(self.apertures[ident])
        else:
            raise ValueError(f'Unrecognized aperture block statement: {statement}')

    def set_current_aperture(self, statement: str):
        # D10*
        # select aperture D10
        match = re.search(r'(D[0-9]{2,})\*', statement)
        if match is not None:
            ident = match.group(1)
            if ident in self.apertures:
                self.current_aperture = ident
            else:
                raise KeyError(f'Aperture {ident} is not defined')
        else:
            raise ValueError(f'Unrecognized set current aperture statement: {statement}')

    def step_and_repeat(self, statement: str):
        # %SRX3Y2I5.0J4.0*%
        # ...
        # %SR*%
        # Step and repeat all enclosed statements
        if statement == '%SR*%':
            self.objects_list_stack.pop()
        else:
            match = re.search(rf'%SRX(\d+)Y(\d+)I({RE_DEC})J({RE_DEC})\*%', statement)
            if match is not None:
                x = int(match.group(1))
                y = int(match.group(2))
                i = float(match.group(3))
                j = float(match.group(4))
                sr = StepAndRepeat(x, y, i, j)
                self.add_object(sr)
                self.objects_list_stack.append(sr)
            else:
                raise ValueError(f'Unrecognized step and repeat statement: {statement}')


class ApertureTransform():
    def __init__(self,
                 polarity: str = 'dark', mirroring: str = 'N',
                 rotation: float = 0.0, scaling: float = 1.0):
        self.polarity = polarity
        self.mirroring = mirroring
        self.rotation = rotation
        self.scaling = scaling


class Aperture():
    def __init__(self):
        pass

    def derive_from(self, statement: str):
        if statement is None:
            raise ValueError('Missing parameters statement')
        tokens = statement.split('X')
        return type(self)(*[float(token) for token in tokens])

    def clone(self):
        new = copy.copy(self)
        return new


class Circle(Aperture):
    def __init__(self, diameter: float, hole_diameter: float = None):
        super().__init__()
        self.diameter = diameter
        self.hole_diameter = hole_diameter


class Rectangle(Aperture):
    def __init__(self, x_size: float, y_size: float,
                 hole_diameter: float = None):
        super().__init__()
        self.x_size = x_size
        self.y_size = y_size
        self.hole_diameter = hole_diameter


class Obround(Aperture):
    def __init__(self, x_size: float, y_size: float, hole_diameter: float = None):
        super().__init__()
        self.x_size = x_size
        self.y_size = y_size
        self.hole_diameter = hole_diameter


class Polygon(Aperture):
    def __init__(self, outer_diameter: float, vertices: int,
                 rotation: float = 0.0, hole_diameter: float = None):
        super().__init__()
        self.outer_diameter = outer_diameter
        self.vertices = int(vertices)
        self.rotation = rotation
        self.hole_diameter = hole_diameter
        if self.vertices not in range(3, 13):
            raise ValueError('Polygon vertices must be from 3 to 12')


class Macro(Aperture):
    def __init__(self, template_str: str, primitives: list):
        super().__init__()
        self.template_str = template_str
        self.primitives = primitives

    def derive_from(self, statement: str):
        # Collect parameter values from creation statement
        params = {}
        if statement is not None:
            for i, token in enumerate(statement.split('X')):
                params[i + 1] = float(token)
        # Create primitives by parsing template string
        primitives = []
        blocks = self.template_str.replace('\n', '').split('*')
        for block in blocks:
            # Ignore open/close block or comment
            if block.startswith('%') or block.startswith('0'):
                continue
            # Resolve new variables
            if block.startswith('$'):
                expr = re.search(r'\$(\d+)\s*=([^*]+)*', block)
                expr_p = expr.group(1)
                expr_e = expr.group(2)
                for p, v in params.items():
                    expr_e = expr_e.replace(f'${p}', str(v))
                params[expr_p] = Macro.eval_expression(expr_e)
            # Attempt to create a primitive
            else:
                code = block.split(',')[0]
                for p, v in params.items():
                    block = block.replace(f'${p}', str(v))
                missing = re.search(r'\$\d+', block)
                if missing:
                    raise KeyError('Unfulfilled macro parameter ' +
                                   missing.group())
                try:
                    primitives.append(Macro.primtypes(code).parse(block))
                except KeyError:
                    raise KeyError('Unrecognized macro code ' + str(code))
        return type(self)(self.template_str, primitives)

    @staticmethod
    def primtypes(code):
        prims = {
            '1': MacroCircle,
            '20': MacroVectorLine,
            '21': MacroCenterLine,
            '4': MacroOutline,
            '5': MacroPolygon,
            '6': MacroMoire,
            '7': MacroThermal
        }
        return prims[code]

    @classmethod
    def parse(cls, statement: str):
        if not statement.startswith('%AM'):
            raise ValueError('Invalid define macro statement')
        # TODO validate template
        return cls(statement, [])

    @staticmethod
    def eval_expression(expr: str):
        legal = set('0123456789()-+/x.')
        chars = set(expr)
        illegal = chars.difference(legal)
        if len(illegal) > 0:
            raise ValueError('Illegal characters in expression: ' + expr)
        expr = expr.replace('x', '*')  # Multiplication
        return eval(expr)


class MacroPrimitive():
    def __init__(self):
        pass

    @classmethod
    def parse(cls, statement: str):
        if statement is None:
            raise ValueError('Missing parameters statement')
        tokens = statement.split(',')[1:]  # Discard first token (shape code)
        return cls(*[Macro.eval_expression(token) for token in tokens])


class MacroCircle(MacroPrimitive):
    def __init__(self, exposure, diameter, x, y, rotation=0.0):
        self.exposure = exposure
        self.diameter = diameter
        self.x = x
        self.y = y
        self.rotation = rotation


class MacroVectorLine(MacroPrimitive):
    def __init__(self, exposure, width, x1, y1, x2, y2, rotation=0.0):
        self.exposure = exposure
        self.width = width
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2
        self.rotation = rotation


class MacroCenterLine(MacroPrimitive):
    def __init__(self, exposure, width, height, x, y, rotation=0.0):
        self.exposure = exposure
        self.width = width
        self.height = height
        self.x = x
        self.y = y
        self.rotation = rotation


class MacroPolygon(MacroPrimitive):
    def __init__(self, exposure, vertices, x, y, diameter, rotation=0.0):
        self.exposure = exposure
        self.vertices = vertices
        self.x = x
        self.y = y
        self.diameter = diameter
        self.rotation = rotation


class MacroThermal(MacroPrimitive):
    def __init__(self, x, y, outer_diameter, inner_diameter, gap, rotation=0.0):
        self.x = x
        self.y = y
        self.outer_diameter = outer_diameter
        self.inner_diameter = inner_diameter
        self.gap = gap
        self.rotation = rotation


class MacroMoire(MacroPrimitive):
    def __init__(self, x, y, outer_diameter, ring_thickness,
                 gap, num_rings, crosshair_thickness, crosshair_length,
                 rotation=0.0):
        self.x = x
        self.y = y
        self.outer_diameter = outer_diameter
        self.ring_thickness = ring_thickness
        self.gap = gap
        self.num_rings = num_rings
        self.crosshair_thickness = crosshair_thickness
        self.crosshair_length = crosshair_length
        self.rotation = rotation


class MacroOutline(MacroPrimitive):
    def __init__(self, exposure, vertices, x, y, *args):
        self.exposure = exposure
        self.vertices = vertices
        self.x = x
        self.y = y
        if len(args) == 2 * vertices + 1:
            self.coordinates = [*args[:-1]]
            self.rotation = float(args[-1])
        else:
            raise ValueError(f'Expected {2*vertices + 1} parameters but received {len(args)}')


class BlockAperture(Aperture):
    def __init__(self):
        self.objects = []

    def append(self, object):
        self.objects.append(object)


class GraphicalObject():
    def __init__(self, aperture, transform, origin: tuple):
        self.aperture = aperture
        self.transform = copy.copy(transform)
        self.origin = origin

    def translate(self, translation):
        dx, dy = translation
        x0, y0 = self.origin
        self.origin = (x0 + dx, y0 + dy)

    def get_vertices(self, scale=1e-6):
        raise NotImplementedError('get_vertices not implemented')


class Draw(GraphicalObject):
    def __init__(self, aperture, transform, origin: tuple, endpoint: tuple):
        super().__init__(aperture, transform, origin)
        self.endpoint = endpoint

    def translate(self, translation):
        dx, dy = translation
        x0, y0 = self.origin
        x1, y1 = self.endpoint
        self.origin = (x0 + dx, y0 + dy)
        self.endpoint = (x1 + dx, y1 + dy)


# TODO Arc needs quadrant mode
class Arc(GraphicalObject):
    def __init__(self, aperture, transform, origin: tuple, endpoint: tuple,
                 offset: tuple, is_cw: bool = True):
        super().__init__(aperture, transform, origin)
        self.endpoint = endpoint
        self.offset = offset
        self.is_cw = is_cw

    def translate(self, translation):
        dx, dy = translation
        x0, y0 = self.origin
        x1, y1 = self.endpoint
        self.origin = (x0 + dx, y0 + dy)
        self.endpoint = (x1 + dx, y1 + dy)


class Flash(GraphicalObject):
    def __init__(self, aperture, transform, origin: tuple):
        super().__init__(aperture, transform, origin)


class Region(GraphicalObject):
    def __init__(self, transform):
        super().__init__(None, transform, None)
        self.objects = []
        self.contours = []

    def end_contour(self):
        if len(self.contours) > 0:
            prev_start, prev_len = self.contours[-1]
            new_start = prev_start + prev_len
            self.contours.append((new_start, len(self.objects) - new_start))
        else:
            self.contours.append((0, len(self.objects)))

    def append(self, object):
        if not isinstance(object, (Draw, Arc)):
            raise TypeError('Region only supports Draw and Arc objects')
        if len(self.objects) > 0 and object.origin != self.objects[-1].endpoint:
            self.end_contour()
        self.objects.append(object)


class StepAndRepeat():
    def __init__(self, nx: int, ny: int, step_x: float, step_y: float):
        if nx < 1 or ny < 1:
            raise ValueError('Repeat must be 1 or greater')
        if step_x < 0.0 or step_y < 0.0:
            raise ValueError('Step size must be positive')
        self.nx = nx
        self.ny = ny
        self.step_x = step_x
        self.step_y = step_y
        self.objects = []

    def append(self, object):
        self.objects.append(object)
