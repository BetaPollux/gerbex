/*
 * test_Rectangle.cpp
 *
 *  Created on: Mar. 24, 2024
 *	Copyright (C) 2024 BetaPollux
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Rectangle.h"
#include "GraphicsStringFrom.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

#define DBL_TOL	1e-9

using namespace gerbex;

TEST_GROUP(RectangleTest) {
};

TEST(RectangleTest, ZeroXSize) {
	CHECK_THROWS(std::invalid_argument, Rectangle(0.0, 1.0));
}

TEST(RectangleTest, ZeroYSize) {
	CHECK_THROWS(std::invalid_argument, Rectangle(1.0, 0.0));
}

TEST(RectangleTest, NegativeHoleSize) {
	CHECK_THROWS(std::invalid_argument, Rectangle(1.0, 1.0, -0.25));
}

TEST(RectangleTest, Vertices) {
	Rectangle rect(1.0, 0.5);

	CHECK_EQUAL(4, rect.GetVertices().size());
	CHECK_EQUAL(Point(0.5, 0.25), rect.GetVertices()[0]);
	CHECK_EQUAL(Point(-0.5, 0.25), rect.GetVertices()[1]);
	CHECK_EQUAL(Point(-0.5, -0.25), rect.GetVertices()[2]);
	CHECK_EQUAL(Point(0.5, -0.25), rect.GetVertices()[3]);
}

TEST(RectangleTest, DefaultHole) {
	Rectangle rect(1.0, 0.5);
	DOUBLES_EQUAL(0.0, rect.GetHoleDiameter(), DBL_TOL);
}

TEST(RectangleTest, Box) {
	double w = 2.0;
	double h = 1.0;
	Rectangle rect(w, h);

	Box expected(w, h, -0.5 * w, -0.5 * h);
	CHECK_EQUAL(expected, rect.GetBox());
}

TEST_GROUP(Rectangle_Transformed) {
	Transform transform;
	Rectangle rect;

	void setup() {
		transform = Transform();
		transform.SetScaling(2.0);
		transform.SetRotation(45.0);

		rect = Rectangle(1.0, 1.0, 0.25);
		rect.ApplyTransform(transform);
	}
};

TEST(Rectangle_Transformed, HoleDiameter) {
	DOUBLES_EQUAL(0.5, rect.GetHoleDiameter(), DBL_TOL);
}

TEST(Rectangle_Transformed, Vertices) {
	// 45-deg rotation with 2x scaling has corners at (0, +/- w) and (+/- w, 0)
	CHECK_EQUAL(4, rect.GetVertices().size());
	CHECK_EQUAL(Point(0.0, sqrt(2.0)), rect.GetVertices()[0]);
	CHECK_EQUAL(Point(-sqrt(2.0), 0.0), rect.GetVertices()[1]);
	CHECK_EQUAL(Point(0.0, -sqrt(2.0)), rect.GetVertices()[2]);
	CHECK_EQUAL(Point(sqrt(2.0), 0.0), rect.GetVertices()[3]);
}

TEST(Rectangle_Transformed, Clone) {
	std::unique_ptr<Aperture> aperture = rect.Clone();
	Rectangle *clone = (Rectangle*) aperture.get();

	CHECK(clone != &rect);
	CHECK_EQUAL(rect, *clone);
}

// TODO test rect serialize
