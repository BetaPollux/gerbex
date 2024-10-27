/*
 * test_MacroOutline.cpp
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
#include "MacroOutline.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(MacroOutlineTest) {
};

TEST(MacroOutlineTest, Default) {
	MacroOutline outline;
	CHECK(outline.GetVertices().size() >= 3);
}

TEST(MacroOutlineTest, Ctor) {
	std::vector<Point> vertices = { Point(-1.0, 0.0), Point(1.0, 0.0), Point(
			0.0, 1.0) };

	MacroOutline outline(MacroExposure::OFF, vertices, 90.0);

	std::vector<Point> expected = { Point(0.0, -1.0), Point(0.0, 1.0), Point(
			-1.0, 0.0) };

	LONGS_EQUAL(MacroExposure::OFF, outline.GetExposure());
	CHECK_EQUAL(3, outline.GetVertices().size());
	CHECK_EQUAL(expected[0], outline.GetVertices()[0]);
	CHECK_EQUAL(expected[1], outline.GetVertices()[1]);
	CHECK_EQUAL(expected[2], outline.GetVertices()[2]);
}

TEST(MacroOutlineTest, CopiesVertices) {
	std::vector<Point> vertices = { Point(-1.0, 0.0), Point(1.0, 0.0), Point(
			0.0, 1.0), };

	MacroOutline outline(MacroExposure::ON, vertices, 0.0);
	vertices.clear();

	CHECK(!outline.GetVertices().empty());
}

TEST(MacroOutlineTest, TooFewVertices) {
	std::vector<Point> vertices(2);
	CHECK_THROWS(std::invalid_argument,
			MacroOutline(MacroExposure::ON, vertices, 0.0));
}

TEST(MacroOutlineTest, EmptyVertices) {
	std::vector<Point> vertices;
	CHECK_THROWS(std::invalid_argument,
			MacroOutline(MacroExposure::ON, vertices, 0.0));
}

TEST(MacroOutlineTest, FromParameters) {
	Parameters params = { 1, 3, 1, -1, 1, 1, 2, 1, 1, -1, 30 };
	std::shared_ptr<MacroOutline> outline = MacroOutline::FromParameters(
			params);
	MacroOutline expected(MacroExposure::ON,
			{ Point(1, -1), Point(1, 1), Point(2, 1), Point(1, -1) }, 30.0);
	CHECK(expected == *outline);
}

TEST(MacroOutlineTest, FromParameters_TooFew) {
	Parameters params = { 1, 3, 1, -1, 1, 1, 2, 1, 1, -1 };
	CHECK_THROWS(std::invalid_argument, MacroOutline::FromParameters(params));
}

TEST(MacroOutlineTest, FromParameters_TooFewXY) {
	Parameters params = { 1, 4, 1, -1, 1, 1, 2, 1, 1, -1, 30 };
	CHECK_THROWS(std::invalid_argument, MacroOutline::FromParameters(params));
}

TEST(MacroOutlineTest, FromParameters_TooManyXY) {
	Parameters params = { 1, 2, 1, -1, 1, 1, 2, 1, 1, -1, 30 };
	CHECK_THROWS(std::invalid_argument, MacroOutline::FromParameters(params));
}

