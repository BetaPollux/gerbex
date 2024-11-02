/*
 * test_MacroThermal.cpp
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

#include "MacroThermal.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"
#include "GraphicsTestHelpers.h"

using namespace gerbex;

TEST_GROUP(MacroThermalTest) {
};

TEST(MacroThermalTest, Default) {
	MacroThermal thermal;

	LONGS_EQUAL(MacroExposure::ON, thermal.GetExposure());
}

TEST(MacroThermalTest, Ctor) {
	Point center(0.5, -0.25);

	MacroThermal thermal(center, 2.5, 1.2, 0.25, 45.0);

	LONGS_EQUAL(MacroExposure::ON, thermal.GetExposure());
	//TODO check contours, especially for corner cases where inner/outer disappear
}

TEST(MacroThermalTest, InnerTooBig) {
	CHECK_THROWS(std::invalid_argument,
			MacroThermal(Point(), 1.0, 1.0, 0.25, 0.0));
}

TEST(MacroThermalTest, GapTooBig) {
	// Gap thickness < (outer diameter)/sqrt(2)
	CHECK_THROWS(std::invalid_argument,
			MacroThermal(Point(), 1.0, 0.5, 0.71, 0.0));
}

TEST(MacroThermalTest, NegativeOuter) {
	CHECK_THROWS(std::invalid_argument,
			MacroThermal(Point(), -1.0, 0.5, 0.25, 0.0));
}

TEST(MacroThermalTest, NegativeInner) {
	CHECK_THROWS(std::invalid_argument,
			MacroThermal(Point(), 1.0, -0.5, 0.25, 0.0));
}

TEST(MacroThermalTest, NegativeGap) {
	CHECK_THROWS(std::invalid_argument,
			MacroThermal(Point(), 1.0, 0.5, -0.25, 0.0));
}

TEST(MacroThermalTest, Thermal) {
	Parameters params = { 0, 0.25, 0.95, 0.75, 0.175, 22.5 };
	std::shared_ptr<MacroThermal> thermal = MacroThermal::FromParameters(
			params);
	MacroThermal expected(Point(0, 0.25), 0.95, 0.75, 0.175, 22.5);
	CHECK(expected == *thermal);
}

TEST(MacroThermalTest, Thermal_TooFewParams) {
	Parameters params = { 0, 0.25, 0.95, 0.75, 0.175 };
	CHECK_THROWS(std::invalid_argument, MacroThermal::FromParameters(params));
}

TEST(MacroThermalTest, Thermal_TooManyParams) {
	Parameters params = { 0, 0.25, 0.95, 0.75, 0.175, 22.5, 5.0 };
	CHECK_THROWS(std::invalid_argument, MacroThermal::FromParameters(params));
}
