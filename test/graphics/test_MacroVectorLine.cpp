/*
 * test_MacroVectorLine.cpp
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
#include "MacroVectorLine.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

#define DBL_TOL 1e-5

TEST_GROUP(MacroVectorLineTest) {
};

TEST(MacroVectorLineTest, Ctor) {
	Point start(0.0, 0.0);
	Point end(5.0, 0.0);

	MacroVectorLine line(MacroExposure::OFF, 2.0, start, end, 0.0);

	std::vector<Point> expected = { Point(0.0, -1.0), Point(0.0, 1.0), Point(
			5.0, 1.0), Point(5.0, -1.0) };
	LONGS_EQUAL(MacroExposure::OFF, line.GetExposure());
	CHECK_EQUAL(4, line.GetVertices().size());
	CHECK_EQUAL(expected[0], line.GetVertices()[0]);
	CHECK_EQUAL(expected[1], line.GetVertices()[1]);
	CHECK_EQUAL(expected[2], line.GetVertices()[2]);
	CHECK_EQUAL(expected[3], line.GetVertices()[3]);
}

TEST(MacroVectorLineTest, Ctor_Rotated) {
	Point start(0.0, 0.0);
	Point end(5.0, 0.0);

	MacroVectorLine line(MacroExposure::OFF, 2.0, start, end, 90.0);

	std::vector<Point> expected = { Point(1.0, 0.0), Point(-1.0, 0.0), Point(
			-1.0, 5.0), Point(1.0, 5.0) };
	LONGS_EQUAL(MacroExposure::OFF, line.GetExposure());
	CHECK_EQUAL(4, line.GetVertices().size());
	CHECK_EQUAL(expected[0], line.GetVertices()[0]);
	CHECK_EQUAL(expected[1], line.GetVertices()[1]);
	CHECK_EQUAL(expected[2], line.GetVertices()[2]);
	CHECK_EQUAL(expected[3], line.GetVertices()[3]);
}

TEST(MacroVectorLineTest, NegativeWidth) {
	CHECK_THROWS(std::invalid_argument,
			MacroVectorLine(MacroExposure::ON, -1.0, Point(), Point(), 0.0));
}

TEST(MacroVectorLineTest, FromParameters) {
	Parameters params = { 1, 0.9, 0, 0.45, 12, 0.75, 22.5 };
	std::shared_ptr<MacroVectorLine> line = MacroVectorLine::FromParameters(
			params);
	MacroVectorLine expected(MacroExposure::ON, 0.9, Point(0.0, 0.45),
			Point(12.0, 0.75), 22.5);
	CHECK(expected == *line);
}

TEST(MacroVectorLineTest, FromParameters_TooFew) {
	Parameters params = { 1, 0.9, 0, 0.45, 12, 0.45 };
	CHECK_THROWS(std::invalid_argument,
			MacroVectorLine::FromParameters(params));
}

TEST(MacroVectorLineTest, FromParameters_TooMany) {
	Parameters params = { 1, 0.9, 0, 0.45, 12, 0.45, 0, 1 };
	CHECK_THROWS(std::invalid_argument,
			MacroVectorLine::FromParameters(params));
}
