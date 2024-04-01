/*
 * test_MacroPolygon.cpp
 *
 *  Created on: Apr. 1, 2024
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

#include "MacroPolygon.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

TEST_GROUP(MacroPolygonTest) {
};

TEST(MacroPolygonTest, Default) {
	MacroPolygon poly;
	CHECK(poly.GetNumVertices() >= 3);
	CHECK(poly.GetNumVertices() <= 12);
	CHECK(poly.GetDiameter() >= 0.0);
}

TEST(MacroPolygonTest, Ctor) {
	RealPoint center(0.5, -0.25);

	MacroPolygon poly(MacroExposure::OFF, 5, center, 1.25, 45.0);

	LONGS_EQUAL(MacroExposure::OFF, poly.GetExposure());
	LONGS_EQUAL(5, poly.GetNumVertices());
	DOUBLES_EQUAL(1.25, poly.GetDiameter(), 1e-9);
	CHECK(center == poly.GetCoord());
	DOUBLES_EQUAL(45.0, poly.GetRotation(), 1e-9);
}

TEST(MacroPolygonTest, TooFewVertices) {
	CHECK_THROWS(std::invalid_argument, MacroPolygon(MacroExposure::ON, 2, RealPoint(), 1.0, 0.0));
}

TEST(MacroPolygonTest, TooManyVertices) {
	CHECK_THROWS(std::invalid_argument, MacroPolygon(MacroExposure::ON, 13, RealPoint(), 1.0, 0.0));
}

