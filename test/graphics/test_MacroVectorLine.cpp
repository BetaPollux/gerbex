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

#include "MacroVectorLine.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

#define DBL_TOL 1e-5

TEST_GROUP(MacroVectorLineTest) {
};

TEST(MacroVectorLineTest, Default) {
	MacroVectorLine line;
	CHECK(line.GetWidth() > 0.0);
}

TEST(MacroVectorLineTest, Ctor) {
	Point start(0.5, -0.25);
	Point end(2.5, -0.5);

	MacroVectorLine line(MacroExposure::OFF, 1.25, start, end, 45.0);

	LONGS_EQUAL(MacroExposure::OFF, line.GetExposure());
	DOUBLES_EQUAL(1.25, line.GetWidth(), 1e-9);
	CHECK(start == line.GetCoord());
	CHECK(end == line.GetEnd());
	DOUBLES_EQUAL(45.0, line.GetRotation(), 1e-9);
}

TEST(MacroVectorLineTest, NegativeWidth) {
	CHECK_THROWS(std::invalid_argument,
			MacroVectorLine(MacroExposure::ON, -1.0, Point(), Point(),
					0.0));
}

TEST(MacroVectorLineTest, FromParameters) {
	Parameters params = { 1, 0.9, 0, 0.45, 12, 0.75, 22.5 };
	std::shared_ptr<MacroVectorLine> line = MacroVectorLine::FromParameters(
			params);
	CHECK(MacroExposure::ON == line->GetExposure());
	DOUBLES_EQUAL(0.9, line->GetWidth(), DBL_TOL);
	DOUBLES_EQUAL(0.0, line->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.45, line->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(12.0, line->GetEnd().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.75, line->GetEnd().GetY(), DBL_TOL);
	DOUBLES_EQUAL(22.5, line->GetRotation(), DBL_TOL);
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
