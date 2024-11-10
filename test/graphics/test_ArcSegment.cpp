/*
 * test_ArcSegment.cpp
 *
 *  Created on: Oct. 15, 2024
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
#include "ArcSegment.h"
#include "CppUTest/TestHarness.h"
#include "GraphicsTestHelpers.h"

#define DBL_TOL	1e-9

namespace gerbex {

TEST_GROUP(ArcSegment) {
	Point start, end, centerOffset;
	ArcDirection direction;
	ArcSegment segment;

	void setup() {
		start = Point(3.0, -2.0);
		end = Point(-3.0, -2.0);
		centerOffset = Point(-3.0, 4.0);
		direction = ArcDirection::CounterClockwise;

		segment = ArcSegment(start, end, centerOffset, direction);
	}
};

TEST(ArcSegment, DefaultCtor) {
	ArcSegment newSegment;

	CHECK(newSegment.GetStart() != newSegment.GetEnd());
}

TEST(ArcSegment, PointsCtor) {
	CHECK_EQUAL(start, segment.GetStart());
	CHECK_EQUAL(end, segment.GetEnd());
	CHECK_EQUAL(centerOffset, segment.GetCenterOffset());
	CHECK_EQUAL((int )direction, (int )segment.GetDirection());
}

TEST(ArcSegment, GetCenter) {
	CHECK_EQUAL(Point(0.0, 2.0), segment.GetCenter());
}

TEST(ArcSegment, GetRadius) {
	DOUBLES_EQUAL(5.0, segment.GetRadius(), DBL_TOL);
}

TEST(ArcSegment, GetBox) {
	//TODO this is for a full circle, not the real arc
	Box expected(10.0, 10.0, -5.0, -3.0);
	CHECK_EQUAL(expected, segment.GetBox());
}

//TODO test transform

} /* namespace gerbex */
