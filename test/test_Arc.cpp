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
#include "Circle.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(ArcTest) {
	Point origin, end, offset;
	std::shared_ptr<Circle> aperture;
	ApertureTransformation transform;
	ArcDirection direction;
	Arc arc;

	void setup() {
		origin = Point(2500, -1500);
		end = Point(3750, 0);
		offset = Point(500, 500);
		aperture = std::make_shared<Circle>(0.75);
		direction = ArcDirection::CounterClockwise;
		transform.SetRotationDegrees(45.0);

		arc = Arc(origin, end, offset, direction, aperture, transform);
	}

};

TEST(ArcTest, Origin) {
	CHECK(origin == arc.GetOrigin());
}

TEST(ArcTest, End) {
	CHECK(end == arc.GetEndPoint());
}

TEST(ArcTest, Offset) {
	CHECK(offset == arc.GetCenterOffset());
}

TEST(ArcTest, Direction) {
	CHECK(direction == arc.GetDirection());
}

TEST(ArcTest, Aperture) {
	CHECK(aperture == arc.GetAperture());
}

TEST(ArcTest, Transform) {
	CHECK(transform == arc.GetTransformation());
}

