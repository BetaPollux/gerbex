/*
 * test_Box.cpp
 *
 *  Created on: Oct 19, 2024
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
#include "Box.h"
#include "CppUTest/TestHarness.h"
#include "GraphicsTestHelpers.h"

#define DBL_TOL	1e-9

namespace gerbex {

TEST_GROUP(Box) {
};

TEST(Box, AspectRatio) {
	Box box(16.0, 9.0, 0.0, 0.0);

	DOUBLES_EQUAL(16.0/9.0, box.GetAspectRatio(), DBL_TOL);
}

TEST(Box, Extend) {
	Box expected(5.0, 4.0, -2.5, -2.0);
	Box box(5.0, 2.0, -2.5, -1.0);
	Box other(4.0, 4.0, -2.0, -2.0);

	CHECK_EQUAL(expected, box.Extend(other));
	CHECK_EQUAL(expected, other.Extend(box));
}

TEST(Box, Extend_Disjoint) {
	Box expected(5.0, 5.0, -1.0, -1.0);
	Box box(1.0, 1.0, -1.0, -1.0);
	Box other(1.0, 1.0, 3.0, 3.0);

	CHECK_EQUAL(expected, box.Extend(other));
	CHECK_EQUAL(expected, other.Extend(box));
}

TEST(Box, Pad) {
	Box expected(4.0, 6.0, -2.0, -3.0);
	Box box(2.0, 4.0, -1.0, -2.0);

	CHECK_EQUAL(expected, box.Pad(1.0));
}

TEST(Box, Translate) {
	Box expected(2.0, 4.0, 1.0, 5.0);
	Box box(2.0, 4.0, -1.0, -2.0);

	CHECK_EQUAL(expected, box.Translate(Point(2.0, 7.0)));
}

} /* namespace gerbex */
