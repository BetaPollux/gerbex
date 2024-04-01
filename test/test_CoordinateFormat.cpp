/*
 * test_CoordinateFormat.cpp
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

#include "CoordinateFormat.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

TEST_GROUP(CoordinateFormatTest) {
};

TEST(CoordinateFormatTest, IntegerTooSmall) {
	CHECK_THROWS(std::invalid_argument, CoordinateFormat(0, 6));
}

TEST(CoordinateFormatTest, IntegerTooLarge) {
	CHECK_THROWS(std::invalid_argument, CoordinateFormat(7, 6));
}

TEST(CoordinateFormatTest, DecimalTooSmall) {
	CHECK_THROWS(std::invalid_argument, CoordinateFormat(3, 4));
}

TEST(CoordinateFormatTest, DecimalTooLarge) {
	CHECK_THROWS(std::invalid_argument, CoordinateFormat(3, 7));
}

TEST(CoordinateFormatTest, Convert_6decimal) {
	Point point(123123456, 23456);
	RealPoint expected(123.123456, 0.023456);

	CoordinateFormat format(3, 6);
	RealPoint actual = format.Convert(point);

	DOUBLES_EQUAL(expected.GetX(), actual.GetX(), 1e-9);
	DOUBLES_EQUAL(expected.GetY(), actual.GetY(), 1e-9);
}

TEST(CoordinateFormatTest, Convert5decimal) {
	Point point(123123456, 23456);
	RealPoint expected(1231.23456, 0.23456);

	CoordinateFormat format(3, 5);
	RealPoint actual = format.Convert(point);

	DOUBLES_EQUAL(expected.GetX(), actual.GetX(), 1e-9);
	DOUBLES_EQUAL(expected.GetY(), actual.GetY(), 1e-9);
}
