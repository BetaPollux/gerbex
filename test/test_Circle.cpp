/*
 * test_Circle.cpp
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

#include "Circle.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

TEST_GROUP(CircleTest) {
};

TEST(CircleTest, NegativeDiameter) {
	CHECK_THROWS(std::invalid_argument, Circle(-0.5));
}

TEST(CircleTest, NegativeHoleDiameter) {
	CHECK_THROWS(std::invalid_argument, Circle(1.25, -0.5));
}

TEST(CircleTest, Diameter) {
	Circle circle(2.3);
	CHECK(2.3 == circle.GetDiameter());
}

TEST(CircleTest, ZeroDiameter) {
	Circle circle(0.0);
	CHECK(0.0 == circle.GetDiameter());
}

TEST(CircleTest, DefaultHole) {
	Circle circle(1.5);
	CHECK(0.0 == circle.GetHoleDiameter());
}

