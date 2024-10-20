/*
 * test_Arc.cpp
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

#include "Arc.h"
#include "GraphicsStringFrom.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(ArcTest) {
	Point start, end, offset;
	std::shared_ptr<Circle> aperture;
	ArcDirection direction;
	Arc arc;

	void setup() {
		start = Point(2.5, -1.5);
		end = Point(3.75, 0.0);
		offset = Point(0.5, 0.5);
		aperture = std::make_shared<Circle>(1.5);
		direction = ArcDirection::CounterClockwise;

		arc = Arc(ArcSegment(start, end, offset, direction), aperture);
	}

};

TEST(ArcTest, Start) {
	CHECK_EQUAL(start, arc.GetSegment().GetStart());
}

TEST(ArcTest, End) {
	CHECK_EQUAL(end, arc.GetSegment().GetEnd());
}

TEST(ArcTest, Offset) {
	CHECK_EQUAL(offset, arc.GetSegment().GetCenterOffset());
}

TEST(ArcTest, Direction) {
	CHECK(direction == arc.GetSegment().GetDirection());
}

TEST(ArcTest, Aperture) {
	CHECK(aperture == arc.GetAperture());
}

TEST(ArcTest, Box) {
	Box expected = arc.GetSegment().GetBox().Pad(0.5 * aperture->GetDiameter());
	CHECK_EQUAL(expected, arc.GetBox());
}

