#!/usr/bin/python3

import numpy as np
import gerber


DEFAULT_MAX_STEP = 10.0


def translate(points: np.ndarray, dx: float, dy: float):
    points += np.array([[dx, dy]])


def circle(diameter: float, *, max_step: float = DEFAULT_MAX_STEP):
    assert diameter > 0.0
    assert max_step <= 90.0
    N = int(360 / max_step)
    angles = np.deg2rad(np.linspace(0, 360, N, endpoint=False))
    x = 0.5 * diameter * np.cos(angles)
    y = 0.5 * diameter * np.sin(angles)
    return np.vstack([x, y]).T


def regular_poly(diameter: float, N: int):
    assert diameter > 0.0
    assert N > 2
    angles = np.linspace(0, 2 * np.pi, N, endpoint=False)
    x = 0.5 * diameter * np.cos(angles)
    y = 0.5 * diameter * np.sin(angles)
    return np.vstack([x, y]).T


def rectangle(width: float, height: float):
    assert width > 0.0
    assert height > 0.0
    return np.array([[0.5 * width, -0.5 * height],
                     [0.5 * width, 0.5 * height],
                     [-0.5 * width, 0.5 * height],
                     [-0.5 * width, -0.5 * height]])


def arc(radius: float, start: float, end: float, *,
        max_step: float = DEFAULT_MAX_STEP):
    assert radius > 0.0
    # TODO handle end > start?
    N = int(1 + np.abs(end - start) / max_step)
    angles = np.linspace(np.deg2rad(start), np.deg2rad(end), N)
    x = radius * np.cos(angles)
    y = radius * np.sin(angles)
    return np.vstack([x, y]).T


def rounded_line(width: float, x1: float, y1: float,
                 x2: float, y2: float, *,
                 max_step: float = DEFAULT_MAX_STEP):
    angle = np.rad2deg(np.arctan2(y2 - y1, x2 - x1))
    r = 0.5 * width
    c1 = arc(r, angle + 90, angle + 270, max_step=max_step)
    translate(c1, x1, y1)
    c2 = arc(r, angle - 90, angle + 90, max_step=max_step)
    translate(c2, x2, y2)
    return np.vstack([c1, c2])


def thick_line(width: float, x1: float, y1: float,
               x2: float, y2: float):
    angle = np.arctan2(y2 - y1, x2 - x1)
    dx = 0.5 * width * np.sin(angle)
    dy = 0.5 * width * np.cos(angle)
    return np.array([[x1 + dx, y1 - dy],
                     [x2 + dx, y2 - dy],
                     [x2 - dx, y2 + dy],
                     [x1 - dx, y1 + dy]])


def rounded_arc(width: float, x0: float, y0: float,
                x1: float, y1: float, x2: float, y2: float, *,
                is_cw: bool = False,
                max_step: float = DEFAULT_MAX_STEP,
                max_step_tip: float = DEFAULT_MAX_STEP):
    dx1, dy1 = x1 - x0, y1 - y0
    dx2, dy2 = x2 - x0, y2 - y0
    r = np.sqrt((dx1)**2 + (dy1)**2)
    dr = 0.5 * width
    start = np.rad2deg(np.arctan2(dy1, dx1))
    end = np.rad2deg(np.arctan2(dy2, dx2))
    if is_cw and end > start:
        end -= 360.0
    elif not is_cw and end < start:
        end += 360.0
    if end == start:
        end = start + 360.0
    tip1 = arc(dr, start - 180, start, max_step=max_step_tip)
    translate(tip1, dx1, dy1)
    outer = arc(r + dr, start, end, max_step=max_step)
    tip2 = arc(dr, end, end + 180, max_step=max_step_tip)
    translate(tip2, dx2, dy2)
    inner = np.flip(arc(r - dr, start, end, max_step=max_step), 1)
    return np.vstack([tip1, outer, tip2, inner])
