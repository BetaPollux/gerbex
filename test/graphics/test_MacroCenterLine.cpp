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

using namespace gerbex;

#define DBL_TOL 1e-5

TEST_GROUP(MacroCenterLineTest) {
};

TEST(MacroCenterLineTest, Default) {
	MacroCenterLine line;
	CHECK(line.GetWidth() > 0.0);
	CHECK(line.GetHeight() > 0.0);
}

TEST(MacroCenterLineTest, Ctor) {
	RealPoint start(0.5, -0.25);

	MacroCenterLine line(MacroExposure::OFF, 1.25, 0.75, start, 45.0);

	LONGS_EQUAL(MacroExposure::OFF, line.GetExposure());
	DOUBLES_EQUAL(1.25, line.GetWidth(), 1e-9);
	DOUBLES_EQUAL(0.75, line.GetHeight(), 1e-9);
	CHECK(start == line.GetCoord());
	DOUBLES_EQUAL(45.0, line.GetRotation(), 1e-9);
}

TEST(MacroCenterLineTest, NegativeWidth) {
	CHECK_THROWS(std::invalid_argument,
			MacroCenterLine(MacroExposure::ON, -1.0, 1.0, RealPoint(), 0.0));
}

TEST(MacroCenterLineTest, NegativeHeight) {
	CHECK_THROWS(std::invalid_argument,
			MacroCenterLine(MacroExposure::ON, 1.0, -1.0, RealPoint(), 0.0));
}

TEST(MacroCenterLineTest, FromParameters) {
	Parameters params = { 1, 6.8, 1.2, 3.4, 0.6, 30 };
	std::shared_ptr<MacroCenterLine> line = MacroCenterLine::FromParameters(
			params);
	CHECK(MacroExposure::ON == line->GetExposure());
	DOUBLES_EQUAL(6.8, line->GetWidth(), DBL_TOL);
	DOUBLES_EQUAL(1.2, line->GetHeight(), DBL_TOL);
	DOUBLES_EQUAL(3.4, line->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.6, line->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(30.0, line->GetRotation(), DBL_TOL);
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
