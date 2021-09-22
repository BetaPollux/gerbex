#!/usr/bin/python3

# RS-274X per standard Revision 2021.02

import re
import copy
import sys

# TBD?
# Processor
# Parser
# State

RE_INT = r'[+-]?[0-9]+'


class Gerber():
    def __init__(self):
        self.format_num_int = None
        self.format_num_dec = None
        self.unit = None
        self.current_point = None
        self.current_aperture = None
        self.interpolation = None
        self.region = None
        self.transform = ApertureTransform()
        self.apertures = {}
        self.templates = {
            'C': Circle,  # TODO make these objects, not constructors
            'R': Rectangle,
            'O': Obround,
            'P': Polygon
        }
        self.macros = {}  # TODO remove this
        self.objects = []

    def comment(self, statement: str):
        pass

    def ignore(self, statement: str):
        pass

    def not_implemented(self, statement: str):
        raise NotImplementedError('Command not implemented: ' + statement)

    def begin_region(self, statement: str):
        self.region = Region(self.transform)

    def end_region(self, statement: str):
        self.region.close()
        self.objects.append(self.region)
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
            'AB': self.not_implemented,
            'SR': self.not_implemented,
            'TF': self.ignore,
            'TA': self.ignore,
            'TO': self.ignore,
            'TD': self.ignore,
            'M02': self.ignore
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

    def parse(self, filename: str):
        with open(filename, 'r') as f:
            delimiter = False
            for line_num, line in enumerate(f):
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
                        print('Error reading ', filename, 'on line',
                              line_num + 1, file=sys.stderr)
                        print(ex, file=sys.stderr)
                        sys.exit()

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
        return self.apertures[self.current_aperture].clone()

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
                if self.region:
                    self.region.append(Draw(None, self.transform, self.current_point, new_point))
                else:
                    self.objects.append(Draw(self.create_aperture(), self.transform,
                                             self.current_point, new_point))
            elif self.interpolation in ('cw_circular', 'ccw_circular'):
                if i and j:
                    offset = (int(i[1:]), int(j[1:]))
                else:
                    raise ValueError(f'Missing offset: I {i}, J {j}')
                is_cw = (self.interpolation == 'cw_circular')
                if self.region:
                    self.region.append(Arc(None, self.transform,
                                           self.current_point, new_point, offset, is_cw))
                else:
                    self.objects.append(Arc(self.create_aperture(), self.transform,
                                            self.current_point, new_point, offset, is_cw))
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
            self.objects.append(Flash(aperture, self.transform, new_point))
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

            # TODO unify macro and templates
            if template_name in self.templates:
                self.apertures[ident] = self.templates[template_name].parse(parameters)
            elif template_name in self.macros:
                self.apertures[ident] = self.macros[template_name].calculate(parameters)
            else:
                raise KeyError(f'Aperture template {template_name} not defined')
        else:
            raise ValueError(f'Unrecognized aperture define statement: {statement}')

    def aperture_macro(self, statement: str):
        # %AMCIRC*\n1,1,1.5,0,0,0*%
        match = re.search(r'%AM([\w\.\$]+)', statement)
        if match is not None:
            ident = match.group(1)
            self.macros[ident] = Macro.parse(statement)
        else:
            raise ValueError(f'Unrecognized aperture macro statement: {statement}')

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

    @classmethod
    def parse(cls, statement: str):  # TODO this should be clone
        if statement is None:
            raise ValueError('Missing parameters statement')
        tokens = statement.split('X')
        return cls(*[float(token) for token in tokens])

    def clone(self):  # TODO add parameters here instead
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
    def __init__(self, name: str, primitives: list):
        super().__init__()
        self.name = name
        self.primitives = primitives

    def calculate(self, parameters: str):
        return self

    @classmethod
    def parse(cls, statement: str):
        primtype = {
            '0': None,  # Comment
            '%': None,  # Delimiter
            '1': MacroCircle,
            '20': MacroVectorLine,
            '21': MacroCenterLine,
            '4': MacroOutline,
            '5': MacroPolygon,
            '6': MacroMoire,
            '7': MacroThermal
        }
        match = re.search(r'%AM([\w\.\$]+)', statement)
        if match:
            name = match.group(1)
        else:
            raise ValueError('Invalid define macro statement')
        primitives = []
        blocks = statement.replace('\n', '').split('*')
        for block in blocks:
            code = block[0]
            try:
                if primtype[code] is not None:
                    primitives.append(primtype[code].parse(block))
            except KeyError:
                raise KeyError('Unrecognized macro code ' + str(code))
        return cls(name, primitives)


class MacroPrimitive():
    def __init__(self):
        pass

    @classmethod
    def parse(cls, statement: str):
        if statement is None:
            raise ValueError('Missing parameters statement')
        tokens = statement.split(',')[1:]  # Discard first token (shape code)
        return cls(*[float(token) for token in tokens])


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


class GraphicalObject():
    def __init__(self, aperture, transform, origin: tuple):
        self.aperture = aperture
        self.transform = copy.copy(transform)
        self.origin = origin

    def get_vertices(self, scale=1e-6):
        raise NotImplementedError('get_vertices not implemented')


class Draw(GraphicalObject):
    def __init__(self, aperture, transform, origin: tuple, endpoint: tuple):
        super().__init__(aperture, transform, origin)
        self.endpoint = endpoint


# TODO Arc needs quadrant mode
class Arc(GraphicalObject):
    def __init__(self, aperture, transform, origin: tuple, endpoint: tuple, offset: tuple, is_cw: bool = True):
        super().__init__(aperture, transform, origin)
        self.endpoint = endpoint
        self.offset = offset
        self.is_cw = is_cw


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

    def close(self):
        self.end_contour()
