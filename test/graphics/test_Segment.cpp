/*
 * test_Segment.cpp
 *
 *  Created on: Mar. 29, 2024
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

#include "Segment.h"
#include "CppUTest/TestHarness.h"
#include "GraphicsTestHelpers.h"

using namespace gerbex;

TEST_GROUP(Segment) {
};

TEST(Segment, DefaultCtor) {
	Segment segment;

	CHECK(segment.GetStart() != segment.GetEnd());
}

TEST(Segment, PointsCtor) {
	Point origin(2500, 5000);
	Point endPoint(7500, 1500);

	Segment segment(origin, endPoint);

	CHECK(origin == segment.GetStart());
	CHECK(endPoint == segment.GetEnd());
}

TEST(Segment, Box) {
	Point start(1.0, 3.0);
	Point end(-2.0, 0.0);
	Box expected(3.0, 3.0, -2.0, 0.0);

	Segment segment(start, end);

	CHECK_EQUAL(expected, segment.GetBox());
}

