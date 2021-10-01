#!/usr/bin/python3

import pytest
from pytest import approx
import numpy as np
import vertices as v


def test_translate():
    pts = np.array([[0.0, 0.0], [1.0, 1.25]])
    v.translate(pts, 0.5, 0.25)
    assert pts == approx(np.array([[0.5, 0.25], [1.5, 1.5]]))


def test_circle():
    pts = v.circle(2.0, max_step=30)
    assert pts.shape == (12, 2)
    assert pts.max() == 1.0
    assert pts.min() == -1.0


def test_regular_poly():
    pts = v.regular_poly(1.0, 3)
    assert pts.shape == (3, 2)
    assert pts[0] == approx([0.5, 0.0], rel=1e-3)
    assert pts[1] == approx([-0.25, 0.433], rel=1e-3)
    assert pts[2] == approx([-0.25, -0.433], rel=1e-3)


def test_rectangle():
    pts = v.rectangle(3.0, 2.5)
    assert pts.shape == (4, 2)
    assert pts[0] == approx([1.5, -1.25])
    assert pts[1] == approx([1.5, 1.25])
    assert pts[2] == approx([-1.5, 1.25])
    assert pts[3] == approx([-1.5, -1.25])


def test_arc_q1():
    pts = v.arc(0.5, 0.0, 90.0, max_step=30.0)
    assert pts.shape == (4, 2)
    assert pts[0] == approx([0.5, 0.0])
    assert pts[1] == approx([0.433, 0.25], rel=1e-3)
    assert pts[2] == approx([0.25, 0.433], rel=1e-3)
    assert pts[3] == approx([0.0, 0.5])


def test_arc_q2():
    pts = v.arc(0.5, 0.0, 120.0, max_step=60.0)
    assert pts.shape == (3, 2)
    assert pts[0] == approx([0.5, 0.0])
    assert pts[1] == approx([0.25, 0.433], rel=1e-3)
    assert pts[2] == approx([-0.25, 0.433], rel=1e-3)


@pytest.mark.parametrize(
    "x2, y2",
    [
        (1, 0),
        (1, 1),
        (0, 1),
        (-1, 1),
        (-1, 0),
        (-1, -1),
        (0, -1),
        (1, -1),
        (0, 0)
    ],
)
def test_rounded_line(x2, y2):
    w = 0.5
    x1, y1 = 0.0, 0.0
    pts = v.rounded_line(w, x1, y1, x2, y2, max_step=5)
    assert pts[:, 0].min() == approx(min(x1, x2) - 0.5 * w, rel=1e-3)
    assert pts[:, 0].max() == approx(max(x1, x2) + 0.5 * w, rel=1e-3)
    assert pts[:, 1].min() == approx(min(y1, y2) - 0.5 * w, rel=1e-3)
    assert pts[:, 1].max() == approx(max(y1, y2) + 0.5 * w, rel=1e-3)


def test_rounded_arc_ccw():
    w = 0.5
    r = 8.0
    x0, y0 = 0.0, 0.0
    x1, y1 = r, 0.0
    x2, y2 = 0.0, r
    pts = v.rounded_arc(w, x0, y0, x1, y1, x2, y2, is_cw=False, max_step=5)
    assert pts[:, 0].min() == approx(-0.5 * w, rel=1e-3)
    assert pts[:, 0].max() == approx(r + 0.5 * w, rel=1e-3)
    assert pts[:, 1].min() == approx(-0.5 * w, rel=1e-3)
    assert pts[:, 1].max() == approx(r + 0.5 * w, rel=1e-3)


def test_rounded_arc_cw():
    w = 0.5
    r = 8.0
    dr = 0.5 * w
    x0, y0 = 0.0, 0.0
    x1, y1 = r, 0.0
    x2, y2 = 0.0, r
    pts = v.rounded_arc(w, x0, y0, x1, y1, x2, y2, is_cw=True, max_step=5)
    assert pts[:, 0].min() == approx(-r - dr, rel=1e-3)
    assert pts[:, 0].max() == approx(r + dr, rel=1e-3)
    assert pts[:, 1].min() == approx(-r - dr, rel=1e-3)
    assert pts[:, 1].max() == approx(r + dr, rel=1e-3)


@pytest.mark.parametrize(
    "is_cw",
    [
        False, True
    ],
)
def test_rounded_arc_circle(is_cw):
    w = 0.5
    r = 8.0
    dr = 0.5 * w
    x0, y0 = 0.0, 0.0
    x1, y1 = r, 0.0
    x2, y2 = r, 0.0
    pts = v.rounded_arc(w, x0, y0, x1, y1, x2, y2, is_cw=is_cw, max_step=5)
    assert pts[:, 0].min() == approx(-r - dr, rel=1e-3)
    assert pts[:, 0].max() == approx(r + dr, rel=1e-3)
    assert pts[:, 1].min() == approx(-r - dr, rel=1e-3)
    assert pts[:, 1].max() == approx(r + dr, rel=1e-3)
