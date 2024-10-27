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
#include "GraphicsStringFrom.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(DrawTest) {
	Point start, end;
	std::shared_ptr<Circle> aperture;
	Draw draw;

	void setup() {
		start = Point(2.5, -1.5);
		end = Point(4.0, 0);
		aperture = std::make_shared<Circle>(1.0);

		draw = Draw(Segment(start, end), aperture);
	}

};

TEST(DrawTest, Origin) {
	CHECK_EQUAL(start, draw.GetSegment().GetStart());
}

TEST(DrawTest, End) {
	CHECK_EQUAL(end, draw.GetSegment().GetEnd());
}

TEST(DrawTest, Aperture) {
	CHECK(aperture == draw.GetAperture());
}

TEST(DrawTest, RejectsNotCircle) {
	std::shared_ptr<Aperture> rect = std::make_shared<Rectangle>();
	CHECK_THROWS(std::invalid_argument, Draw(Segment(start, end), rect));
}

TEST(DrawTest, Box) {
	Box expected(2.5, 2.5, 2.0, -2.0);
	CHECK_EQUAL(expected, draw.GetBox());
}

TEST(DrawTest, ApplyTransform) {
	Transform transform(Mirroring::X, 30.0, 0.5);
	Circle expectedCircle(*aperture);
	expectedCircle.ApplyTransform(transform);
	Segment expectedSegment(start, end);
	expectedSegment.ApplyTransform(transform);

	draw.ApplyTransform(transform);

	CHECK_EQUAL(expectedCircle, *draw.GetAperture());
	CHECK(expectedSegment == draw.GetSegment());
}

TEST(DrawTest, Translate) {
	Point offset(10.0, -15.0);

	Segment expectedSegment(start, end);
	expectedSegment.Translate(offset);

	draw.Translate(offset);

	CHECK(expectedSegment == draw.GetSegment());
}

//TODO test draw serialize
