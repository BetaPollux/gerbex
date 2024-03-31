/*
 * test_Obround.cpp
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

#include "Obround.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

TEST_GROUP(ObroundTest) {
};

TEST(ObroundTest, ZeroXSize) {
	CHECK_THROWS(std::invalid_argument, Obround(0.0, 1.0));
}

TEST(ObroundTest, ZeroYSize) {
	CHECK_THROWS(std::invalid_argument, Obround(1.0, 0.0));
}

TEST(ObroundTest, NegativeHoleSize) {
	CHECK_THROWS(std::invalid_argument, Obround(1.0, 1.0, -0.25));
}

TEST(ObroundTest, XSize) {
	Obround obround(1.0, 0.5);
	CHECK(1.0 == obround.GetXSize());
}

TEST(ObroundTest, YSize) {
	Obround obround(1.0, 0.5);
	CHECK(0.5 == obround.GetYSize());
}

TEST(ObroundTest, DefaultHole) {
	Obround obround(1.0, 0.5);
	CHECK(0.0 == obround.GetHoleDiameter());
}

