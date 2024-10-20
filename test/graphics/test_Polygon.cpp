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
	CHECK(0.0 == poly.GetRotation());
}

TEST(PolygonTest, Diameter) {
	Polygon poly(2.5, 3);
	CHECK(2.5 == poly.GetOuterDiameter());
}

TEST(PolygonTest, Rotation) {
	Polygon poly(1.0, 3, 45.0);
	CHECK(45.0 == poly.GetRotation());
}

TEST(PolygonTest, DefaultHole) {
	Polygon poly(1.0, 3);
	CHECK(0.0 == poly.GetHoleDiameter());
}

TEST(PolygonTest, Box) {
	double d = 5.0;
	Polygon poly(d, 7, 0.0);

	Box expected(d, d, -0.5 * d, -0.5 * d);
	CHECK_EQUAL(expected, poly.GetBox());
}
