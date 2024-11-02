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
#include "GraphicsTestHelpers.h"

using namespace gerbex;

#define DBL_TOL 1e-9

TEST_GROUP(MacroCircleTest) {
};

TEST(MacroCircleTest, Default) {
	MacroCircle circle;
	CHECK(circle.GetDiameter() > 0.0);
}

TEST(MacroCircleTest, Ctor) {
	Point center(0.5, -0.25);

	MacroCircle circle(MacroExposure::OFF, 1.25, center, 45.0);

	Point expected = center;
	expected.Rotate(45.0);
	LONGS_EQUAL(MacroExposure::OFF, circle.GetExposure());
	DOUBLES_EQUAL(1.25, circle.GetDiameter(), DBL_TOL);
	CHECK_EQUAL(expected, circle.GetCenter());
}

TEST(MacroCircleTest, NegativeDiameter) {
	CHECK_THROWS(std::invalid_argument,
			MacroCircle(MacroExposure::ON, -1.0, Point()));
}

TEST(MacroCircleTest, FromParameters) {
	Parameters params = { 1.0, 1.5, -3.0, 2.0, 45.0 };
	std::shared_ptr<MacroCircle> circle = MacroCircle::FromParameters(params);
	MacroCircle expected(MacroExposure::ON, 1.5, Point(-3.0, 2.0), 45.0);
	CHECK(expected == *circle);
}

TEST(MacroCircleTest, FromParameters_NoRotation) {
	Parameters params = { 1.0, 1.5, -3.0, 2.0 };
	std::shared_ptr<MacroCircle> circle = MacroCircle::FromParameters(params);
	MacroCircle expected(MacroExposure::ON, 1.5, Point(-3.0, 2.0));
	CHECK(expected == *circle);
}

TEST(MacroCircleTest, FromParameters_TooFew) {
	Parameters params = { 1.0, 1.5, -3.0 };
	CHECK_THROWS(std::invalid_argument, MacroCircle::FromParameters(params));
}

TEST(MacroCircleTest, FromParameters_TooMany) {
	Parameters params = { 1.0, 1.5, -3.0, 2.0, 45.0, 23.0 };
	CHECK_THROWS(std::invalid_argument, MacroCircle::FromParameters(params));
}
