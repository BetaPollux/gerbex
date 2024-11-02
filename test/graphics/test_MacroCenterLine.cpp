/*
 * test_MacroCenterLine.cpp
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

#include "MacroCenterLine.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"
#include "GraphicsTestHelpers.h"

using namespace gerbex;

TEST_GROUP(MacroCenterLineTest) {
};

TEST(MacroCenterLineTest, Ctor) {
	Point center(10.0, 5.0);

	MacroCenterLine line(MacroExposure::OFF, 4.0, 2.0, center, 0.0);

	std::vector<Point> expected = { Point(12.0, 6.0), Point(8.0, 6.0), Point(
			8.0, 4.0), Point(12.0, 4.0) };
	LONGS_EQUAL(MacroExposure::OFF, line.GetExposure());
	CHECK_EQUAL(4, line.GetVertices().size());
	CHECK_EQUAL(expected[0], line.GetVertices()[0]);
	CHECK_EQUAL(expected[1], line.GetVertices()[1]);
	CHECK_EQUAL(expected[2], line.GetVertices()[2]);
	CHECK_EQUAL(expected[3], line.GetVertices()[3]);
}

TEST(MacroCenterLineTest, Ctor_Rotated) {
	Point center(10.0, 5.0);

	MacroCenterLine line(MacroExposure::OFF, 4.0, 2.0, center, 90.0);

	std::vector<Point> expected = { Point(-6.0, 12.0), Point(-6.0, 8.0), Point(
			-4.0, 8.0), Point(-4.0, 12.0) };
	LONGS_EQUAL(MacroExposure::OFF, line.GetExposure());
	CHECK_EQUAL(4, line.GetVertices().size());
	CHECK_EQUAL(expected[0], line.GetVertices()[0]);
	CHECK_EQUAL(expected[1], line.GetVertices()[1]);
	CHECK_EQUAL(expected[2], line.GetVertices()[2]);
	CHECK_EQUAL(expected[3], line.GetVertices()[3]);
}

TEST(MacroCenterLineTest, NegativeWidth) {
	CHECK_THROWS(std::invalid_argument,
			MacroCenterLine(MacroExposure::ON, -1.0, 1.0, Point(), 0.0));
}

TEST(MacroCenterLineTest, NegativeHeight) {
	CHECK_THROWS(std::invalid_argument,
			MacroCenterLine(MacroExposure::ON, 1.0, -1.0, Point(), 0.0));
}

TEST(MacroCenterLineTest, FromParameters) {
	Parameters params = { 1, 6.8, 1.2, 3.4, 0.6, 30 };
	std::shared_ptr<MacroCenterLine> line = MacroCenterLine::FromParameters(
			params);

	MacroCenterLine expected(MacroExposure::ON, 6.8, 1.2, Point(3.4, 0.6), 30);
	CHECK(MacroExposure::ON == line->GetExposure());
	CHECK(expected == *line);
}

TEST(MacroCenterLineTest, FromParameters_TooFew) {
	Parameters params = { 1, 6.8, 1.2, 3.4, 0.6 };
	CHECK_THROWS(std::invalid_argument,
			MacroCenterLine::FromParameters(params));
}

TEST(MacroCenterLineTest, FromParameters_TooMany) {
	Parameters params = { 1, 6.8, 1.2, 3.4, 0.6, 30, 5 };
	CHECK_THROWS(std::invalid_argument,
			MacroCenterLine::FromParameters(params));
}
