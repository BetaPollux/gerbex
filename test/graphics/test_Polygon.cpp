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
#include "GraphicsStringFrom.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

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

TEST(PolygonTest, DefaultRotation) {
	Polygon poly(1.0, 3);
	DOUBLES_EQUAL(0.0, poly.GetRotation(), DBL_TOL);
}

TEST(PolygonTest, Diameter) {
	Polygon poly(2.5, 3);
	DOUBLES_EQUAL(2.5, poly.GetOuterDiameter(), DBL_TOL);
}

TEST(PolygonTest, Rotation) {
	Polygon poly(1.0, 3, 45.0);
	DOUBLES_EQUAL(45.0, poly.GetRotation(), DBL_TOL);
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
		poly.SetTransform(transform);
	}
};

TEST(Polygon_Transformed, OuterDiameter) {
	DOUBLES_EQUAL(4.0, poly.GetOuterDiameter(), DBL_TOL);
}

TEST(Polygon_Transformed, Rotation) {
	DOUBLES_EQUAL(90.0, poly.GetRotation(), DBL_TOL);
}

TEST(Polygon_Transformed, HoleDiameter) {
	DOUBLES_EQUAL(1.0, poly.GetHoleDiameter(), DBL_TOL);
}

TEST(Polygon_Transformed, Box) {
	// 90-deg rotation has corners at (0, +/- d) and (+/- d, 0)
	Box expected(poly.GetOuterDiameter(), Point());
	Box box = poly.GetBox();
	DOUBLES_EQUAL(expected.GetWidth(), box.GetWidth(), DBL_TOL);
	DOUBLES_EQUAL(expected.GetHeight(), box.GetHeight(), DBL_TOL);
	DOUBLES_EQUAL(expected.GetLeft(), box.GetLeft(), DBL_TOL);
	DOUBLES_EQUAL(expected.GetBottom(), box.GetBottom(), DBL_TOL);
}

TEST(Polygon_Transformed, Clone) {
	std::unique_ptr<Aperture> aperture = poly.Clone();
	Polygon *clone = (Polygon*) aperture.get();

	CHECK(clone != &poly);
	DOUBLES_EQUAL(clone->GetOuterDiameter(), poly.GetOuterDiameter(), DBL_TOL);
	LONGS_EQUAL(clone->GetNumVertices(), poly.GetNumVertices());
	DOUBLES_EQUAL(clone->GetRotation(), poly.GetRotation(), DBL_TOL);
	DOUBLES_EQUAL(clone->GetHoleDiameter(), poly.GetHoleDiameter(), DBL_TOL);
	CHECK(clone->GetTransform() == poly.GetTransform());
}

// TODO test poly serialize

