/*
 * test_MacroPrimitive.cpp
 *
 *  Created on: Sep. 26, 2024
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

#include "MacroPrimitive.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(MacroPrimitiveTest) {
};

TEST(MacroPrimitiveTest, ExposureFromStr) {
	CHECK(MacroExposure::OFF == MacroPrimitive::ExposureFromNum(0));
	CHECK(MacroExposure::ON == MacroPrimitive::ExposureFromNum(1));
	CHECK_THROWS(std::invalid_argument, MacroPrimitive::ExposureFromNum(2));
}
