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

TEST_GROUP(MacroPolygonTest) {
};

TEST(MacroPolygonTest, Default) {
	MacroPolygon poly;
	CHECK(poly.GetVertices().size() >= 3);
	CHECK(poly.GetVertices().size() <= 12);
}

TEST(MacroPolygonTest, Ctor) {
	Point center(2.0, 5.0);

	MacroPolygon poly(MacroExposure::OFF, 3, center, 2.0, 0.0);

	std::vector<Point> expected = { Point(3.0, 5.0), Point(1.5,
			5.0 + sqrt(3.0 / 4.0)), Point(1.5, 5.0 - sqrt(3.0 / 4.0)) };

	LONGS_EQUAL(MacroExposure::OFF, poly.GetExposure());
	CHECK_EQUAL(3, poly.GetVertices().size());
	CHECK_EQUAL(expected[0], poly.GetVertices()[0]);
	CHECK_EQUAL(expected[1], poly.GetVertices()[1]);
	CHECK_EQUAL(expected[2], poly.GetVertices()[2]);
}

TEST(MacroPolygonTest, Ctor_Rotated) {
	Point center(2.0, 5.0);

	MacroPolygon poly(MacroExposure::OFF, 3, center, 2.0, 90.0);

	std::vector<Point> expected = { Point(-5.0, 3.0), Point(
			-5.0 - sqrt(3.0 / 4.0), 1.5), Point(-5.0 + sqrt(3.0 / 4.0), 1.5) };

	LONGS_EQUAL(MacroExposure::OFF, poly.GetExposure());
	CHECK_EQUAL(3, poly.GetVertices().size());
	CHECK_EQUAL(expected[0], poly.GetVertices()[0]);
	CHECK_EQUAL(expected[1], poly.GetVertices()[1]);
	CHECK_EQUAL(expected[2], poly.GetVertices()[2]);
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
	MacroPolygon expected(MacroExposure::ON, 8, Point(3, 4), 6, 12);
	CHECK(expected == *poly);
}

TEST(MacroPolygonTest, FromParameters_TooFew) {
	Parameters params = { 1, 8, 0, 0, 8 };
	CHECK_THROWS(std::invalid_argument, MacroPolygon::FromParameters(params));
}

TEST(MacroPolygonTest, FromParameters_TooMany) {
	Parameters params = { 5, 1, 8, 0, 0, 8, 0, 1.0 };
	CHECK_THROWS(std::invalid_argument, MacroPolygon::FromParameters(params));
}

