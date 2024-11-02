/*
 * test_Polygon.cpp
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

#include "Polygon.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"
#include "GraphicsTestHelpers.h"

#define DBL_TOL	1e-9

using namespace gerbex;

TEST_GROUP(PolygonTest) {
};

TEST(PolygonTest, ZeroDiameter) {
	CHECK_THROWS(std::invalid_argument, Polygon(0.0, 3));
}

TEST(PolygonTest, TooFewVertices) {
	CHECK_THROWS(std::invalid_argument, Polygon(1.0, 2));
}

TEST(PolygonTest, TooManyVertices) {
	CHECK_THROWS(std::invalid_argument, Polygon(1.0, 13));
}

TEST(PolygonTest, NegativeHoleSize) {
	CHECK_THROWS(std::invalid_argument, Polygon(1.0, 3, 0.0, -0.25));
}

TEST(PolygonTest, Vertices) {
	Polygon poly(1.0, 3);
	double r = 0.5;
	double hr = r / 2.0;

	CHECK_EQUAL(3, poly.GetVertices().size());
	CHECK_EQUAL(Point(r, 0.0), poly.GetVertices()[0]);
	CHECK_EQUAL(Point(-hr, sqrt(r * r - hr * hr)), poly.GetVertices()[1]);
	CHECK_EQUAL(Point(-hr, -sqrt(r * r - hr * hr)), poly.GetVertices()[2]);
}

TEST(PolygonTest, DefaultHole) {
	Polygon poly(1.0, 3);
	DOUBLES_EQUAL(0.0, poly.GetHoleDiameter(), DBL_TOL);
}

TEST(PolygonTest, Box) {
	double d = 5.0;
	Polygon poly(d, 4, 0.0);

	Box expected(d, Point());
	CHECK_EQUAL(expected, poly.GetBox());
}

TEST_GROUP(Polygon_Transformed) {
	Transform transform;
	Polygon poly;

	void setup() {
		transform = Transform();
		transform.SetScaling(2.0);
		transform.SetRotation(45.0);

		poly = Polygon(2.0, 4, 45.0, 0.5);
		poly.ApplyTransform(transform);
	}
};

TEST(Polygon_Transformed, HoleDiameter) {
	DOUBLES_EQUAL(1.0, poly.GetHoleDiameter(), DBL_TOL);
}

TEST(Polygon_Transformed, Vertices) {
	// 45+45-deg rotation with 2x scaling has corners at (0, +/- d) and (+/- d, 0)
	CHECK_EQUAL(4, poly.GetVertices().size());
	CHECK_EQUAL(Point(0.0, 2.0), poly.GetVertices()[0]);
	CHECK_EQUAL(Point(-2.0, 0.0), poly.GetVertices()[1]);
	CHECK_EQUAL(Point(0.0, -2.0), poly.GetVertices()[2]);
	CHECK_EQUAL(Point(2.0, 0.0), poly.GetVertices()[3]);
}

TEST(Polygon_Transformed, Clone) {
	std::unique_ptr<Aperture> aperture = poly.Clone();
	Polygon *clone = (Polygon*) aperture.get();

	CHECK(clone != &poly);
	CHECK_EQUAL(poly, *clone);
}

// TODO test poly serialize

