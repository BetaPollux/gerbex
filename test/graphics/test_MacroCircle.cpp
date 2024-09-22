/*
 * test_MacroCircle.cpp
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

#include "MacroCircle.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;


TEST_GROUP(MacroCircleTest) {
};

TEST(MacroCircleTest, Default) {
	MacroCircle circle;
	CHECK(circle.GetDiameter() > 0.0);
}

TEST(MacroCircleTest, Ctor) {
	RealPoint center(0.5, -0.25);

	MacroCircle circle(MacroExposure::OFF, 1.25, center, 45.0);

	LONGS_EQUAL(MacroExposure::OFF, circle.GetExposure());
	DOUBLES_EQUAL(1.25, circle.GetDiameter(), 1e-9);
	CHECK(center == circle.GetCoord());
	DOUBLES_EQUAL(45.0, circle.GetRotation(), 1e-9);
}

TEST(MacroCircleTest, NegativeDiameter) {
	CHECK_THROWS(std::invalid_argument, MacroCircle(MacroExposure::ON, -1.0, RealPoint()));
}

