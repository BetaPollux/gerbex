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
#include "GraphicsStringFrom.h"
#include "CppUTest/TestHarness.h"

namespace gerbex {

TEST_GROUP(ArcSegment) {
};

TEST(ArcSegment, DefaultCtor) {
	ArcSegment segment;

	CHECK(segment.GetStart() != segment.GetEnd());
}

TEST(ArcSegment, PointsCtor) {
	Point start(2500, 5000);
	Point end(7500, 1500);
	Point centerOffset(500, -200);
	ArcDirection direction = ArcDirection::CounterClockwise;

	ArcSegment segment(start, end, centerOffset, direction);

	CHECK_EQUAL(start, segment.GetStart());
	CHECK_EQUAL(end, segment.GetEnd());
	CHECK_EQUAL(centerOffset, segment.GetCenterOffset());
	CHECK_EQUAL((int )direction, (int )segment.GetDirection());
}


TEST(ArcSegment, GetCenter) {
	Point start(2500, 5000);
	Point end(7500, 1500);
	Point centerOffset(500, -200);

	ArcDirection direction = ArcDirection::CounterClockwise;

	ArcSegment segment(start, end, centerOffset, direction);

	CHECK_EQUAL(Point(3000, 4800), segment.GetCenter());
}

} /* namespace gerbex */
