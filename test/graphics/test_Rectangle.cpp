/*
 * test_Rectangle.cpp
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

#include "Rectangle.h"
#include "GraphicsStringFrom.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;


TEST_GROUP(RectangleTest) {
};

TEST(RectangleTest, ZeroXSize) {
	CHECK_THROWS(std::invalid_argument, Rectangle(0.0, 1.0));
}

TEST(RectangleTest, ZeroYSize) {
	CHECK_THROWS(std::invalid_argument, Rectangle(1.0, 0.0));
}

TEST(RectangleTest, NegativeHoleSize) {
	CHECK_THROWS(std::invalid_argument, Rectangle(1.0, 1.0, -0.25));
}

TEST(RectangleTest, XSize) {
	Rectangle rect(1.0, 0.5);
	CHECK(1.0 == rect.GetXSize());
}

TEST(RectangleTest, YSize) {
	Rectangle rect(1.0, 0.5);
	CHECK(0.5 == rect.GetYSize());
}

TEST(RectangleTest, DefaultHole) {
	Rectangle rect(1.0, 0.5);
	CHECK(0.0 == rect.GetHoleDiameter());
}

TEST(RectangleTest, Box) {
	double w = 2.0;
	double h = 1.0;
	Rectangle rect(w, h);

	Box expected(w, h, -0.5 * w, -0.5 * h);
	CHECK_EQUAL(expected, rect.GetBox());
}

