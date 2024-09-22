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
	std::vector<RealPoint> vertices = {
			RealPoint(-1.0, 0.0),
			RealPoint(1.0, 0.0),
			RealPoint(0.0, 1.0),
	};

	MacroOutline outline(MacroExposure::OFF, vertices, 45.0);

	LONGS_EQUAL(MacroExposure::OFF, outline.GetExposure());
	CHECK(vertices == outline.GetVertices());
	DOUBLES_EQUAL(45.0, outline.GetRotation(), 1e-9);
}

TEST(MacroOutlineTest, CopiesVertices) {
	std::vector<RealPoint> vertices = {
			RealPoint(-1.0, 0.0),
			RealPoint(1.0, 0.0),
			RealPoint(0.0, 1.0),
	};

	MacroOutline outline(MacroExposure::ON, vertices, 0.0);
	vertices.clear();

	CHECK(RealPoint(-1.0, 0.0) == outline.GetVertices()[0]);
	CHECK(RealPoint(1.0, 0.0) == outline.GetVertices()[1]);
	CHECK(RealPoint(0.0, 1.0) == outline.GetVertices()[2]);
}

TEST(MacroOutlineTest, TooFewVertices) {
	std::vector<RealPoint> vertices(2);
	CHECK_THROWS(std::invalid_argument, MacroOutline(MacroExposure::ON, vertices, 0.0));
}

TEST(MacroOutlineTest, EmptyVertices) {
	std::vector<RealPoint> vertices;
	CHECK_THROWS(std::invalid_argument, MacroOutline(MacroExposure::ON, vertices, 0.0));
}


