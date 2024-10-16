/*
 * test_Draw.cpp
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

#include "Circle.h"
#include "Draw.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(DrawTest) {
	Point start, end;
	std::shared_ptr<Circle> aperture;
	ApertureTransformation transform;
	Draw draw;

	void setup() {
		start = Point(2500, -1500);
		end = Point(3750, 0);
		aperture = std::make_shared<Circle>(0.75);
		transform.SetRotationDegrees(45.0);

		draw = Draw(Segment(start, end), aperture, transform);
	}

};

TEST(DrawTest, Origin) {
	CHECK(start == draw.GetSegment().GetStart());
}

TEST(DrawTest, End) {
	CHECK(end == draw.GetSegment().GetEnd());
}

TEST(DrawTest, Aperture) {
	CHECK(aperture == draw.GetAperture());
}

TEST(DrawTest, Transform) {
	CHECK(transform == draw.GetTransform());
}
