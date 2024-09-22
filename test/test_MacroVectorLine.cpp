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


TEST_GROUP(MacroVectorLineTest) {
};

TEST(MacroVectorLineTest, Default) {
	MacroVectorLine line;
	CHECK(line.GetWidth() > 0.0);
}

TEST(MacroVectorLineTest, Ctor) {
	RealPoint start(0.5, -0.25);
	RealPoint end(2.5, -0.5);

	MacroVectorLine line(MacroExposure::OFF, 1.25, start, end, 45.0);

	LONGS_EQUAL(MacroExposure::OFF, line.GetExposure());
	DOUBLES_EQUAL(1.25, line.GetWidth(), 1e-9);
	CHECK(start == line.GetCoord());
	CHECK(end == line.GetEnd());
	DOUBLES_EQUAL(45.0, line.GetRotation(), 1e-9);
}

TEST(MacroVectorLineTest, NegativeWidth) {
	CHECK_THROWS(std::invalid_argument, MacroVectorLine(MacroExposure::ON, -1.0, RealPoint(), RealPoint(), 0.0));
}

