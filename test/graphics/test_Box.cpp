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

#define DBL_TOL 1e-9

namespace gerbex {

TEST_GROUP(Box) {
};

TEST(Box, Extend) {
	Box one(5.0, 2.0, -2.5, -1.0);
	Box two(4.0, 4.0, -2.0, -2.0);
	Box three = one.Extend(two);

	DOUBLES_EQUAL(5.0, three.GetWidth(), DBL_TOL);
	DOUBLES_EQUAL(4.0, three.GetHeight(), DBL_TOL);
	DOUBLES_EQUAL(-2.5, three.GetLeft(), DBL_TOL);
	DOUBLES_EQUAL(-2.0, three.GetBottom(), DBL_TOL);
}

} /* namespace gerbex */
