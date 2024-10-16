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

#include "GraphicsStringFrom.h"
#include "MacroPolygon.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

#define DBL_TOL 1e-5

TEST_GROUP(MacroPolygonTest) {
};

TEST(MacroPolygonTest, Default) {
	MacroPolygon poly;
	CHECK(poly.GetNumVertices() >= 3);
	CHECK(poly.GetNumVertices() <= 12);
	CHECK(poly.GetDiameter() >= 0.0);
}

TEST(MacroPolygonTest, Ctor) {
	Point center(0.5, -0.25);

	MacroPolygon poly(MacroExposure::OFF, 5, center, 1.25, 45.0);

	LONGS_EQUAL(MacroExposure::OFF, poly.GetExposure());
	LONGS_EQUAL(5, poly.GetNumVertices());
	DOUBLES_EQUAL(1.25, poly.GetDiameter(), 1e-9);
	CHECK_EQUAL(center, poly.GetCenter());
	DOUBLES_EQUAL(45.0, poly.GetRotation(), 1e-9);
}

TEST(MacroPolygonTest, TooFewVertices) {
	CHECK_THROWS(std::invalid_argument,
			MacroPolygon(MacroExposure::ON, 2, Point(), 1.0, 0.0));
}

TEST(MacroPolygonTest, TooManyVertices) {
	CHECK_THROWS(std::invalid_argument,
			MacroPolygon(MacroExposure::ON, 13, Point(), 1.0, 0.0));
}

TEST(MacroPolygonTest, FromParameters) {
	Parameters params = { 1, 8, 3, 4, 6, 12 };
	std::shared_ptr<MacroPolygon> poly = MacroPolygon::FromParameters(params);
	CHECK(MacroExposure::ON == poly->GetExposure());
	LONGS_EQUAL(8, poly->GetNumVertices());
	DOUBLES_EQUAL(3.0, poly->GetCenter().GetX(), DBL_TOL);
	DOUBLES_EQUAL(4.0, poly->GetCenter().GetY(), DBL_TOL);
	DOUBLES_EQUAL(6.0, poly->GetDiameter(), DBL_TOL);
	DOUBLES_EQUAL(12.0, poly->GetRotation(), DBL_TOL);
}

TEST(MacroPolygonTest, FromParameters_TooFew) {
	Parameters params = { 1, 8, 0, 0, 8 };
	CHECK_THROWS(std::invalid_argument, MacroPolygon::FromParameters(params));
}

TEST(MacroPolygonTest, FromParameters_TooMany) {
	Parameters params = { 5, 1, 8, 0, 0, 8, 0, 1.0 };
	CHECK_THROWS(std::invalid_argument, MacroPolygon::FromParameters(params));
}

