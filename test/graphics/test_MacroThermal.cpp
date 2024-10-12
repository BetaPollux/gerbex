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

using namespace gerbex;

#define DBL_TOL 1e-5

TEST_GROUP(MacroThermalTest) {
};

TEST(MacroThermalTest, Default) {
	MacroThermal thermal;

	LONGS_EQUAL(MacroExposure::ON, thermal.GetExposure());
	CHECK(thermal.GetOuterDiameter() > thermal.GetInnerDiameter());
	CHECK(thermal.GetOuterDiameter() > 0.0);
	CHECK(thermal.GetInnerDiameter() >= 0.0);
	CHECK(thermal.GetGapThickness() >= 0.0);
	CHECK(thermal.GetGapThickness() < thermal.GetOuterDiameter() / 1.4142);
}

TEST(MacroThermalTest, Ctor) {
	Point center(0.5, -0.25);

	MacroThermal thermal(center, 2.5, 1.2, 0.25, 45.0);

	LONGS_EQUAL(MacroExposure::ON, thermal.GetExposure());
	DOUBLES_EQUAL(2.5, thermal.GetOuterDiameter(), 1e-9);
	DOUBLES_EQUAL(1.2, thermal.GetInnerDiameter(), 1e-9);
	DOUBLES_EQUAL(0.25, thermal.GetGapThickness(), 1e-9);
	CHECK(center == thermal.GetCoord());
	DOUBLES_EQUAL(45.0, thermal.GetRotation(), 1e-9);
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
	CHECK(MacroExposure::ON == thermal->GetExposure());
	DOUBLES_EQUAL(0.0, thermal->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.25, thermal->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(0.95, thermal->GetOuterDiameter(), DBL_TOL);
	DOUBLES_EQUAL(0.75, thermal->GetInnerDiameter(), DBL_TOL);
	DOUBLES_EQUAL(0.175, thermal->GetGapThickness(), DBL_TOL);
	DOUBLES_EQUAL(22.5, thermal->GetRotation(), DBL_TOL);
}

TEST(MacroThermalTest, Thermal_TooFewParams) {
	Parameters params = { 0, 0.25, 0.95, 0.75, 0.175 };
	CHECK_THROWS(std::invalid_argument, MacroThermal::FromParameters(params));
}

TEST(MacroThermalTest, Thermal_TooManyParams) {
	Parameters params = { 0, 0.25, 0.95, 0.75, 0.175, 22.5, 5.0 };
	CHECK_THROWS(std::invalid_argument, MacroThermal::FromParameters(params));
}
