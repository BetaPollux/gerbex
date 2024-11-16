/*
 * test_RegionContour.cpp
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

#include "ArcSegment.h"
#include "Circle.h"
#include "Contour.h"
#include "GraphicsTestHelpers.h"
#include <memory>
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(ContourTest) {
	Contour contour;

	void setup() {
		contour = Contour();
	}
};

TEST(ContourTest, Initial) {
	LONGS_EQUAL(0, contour.GetSegments().size());
}

TEST(ContourTest, AddSegment) {
	std::shared_ptr<Segment> segment = std::make_shared<Segment>(Point(0, 0),
			Point(0, 100));

	contour.AddSegment(segment);

	LONGS_EQUAL(1, contour.GetSegments().size());
	POINTERS_EQUAL(segment.get(), contour.GetSegments().back().get());
}

TEST(ContourTest, AddSegment_ZeroLength) {
	Point pt1 = Point(0, 0);
	Point pt2 = Point(0, 0);

	CHECK_THROWS(std::invalid_argument,
			contour.AddSegment(std::make_shared<Segment>(pt1, pt2)));
}

TEST(ContourTest, IsClosed_Empty) {
	CHECK(!contour.IsClosed());
}

TEST(ContourTest, IsCircle_Empty) {
	CHECK(!contour.IsCircle());
}

TEST(ContourTest, IsClosed_TwoSegments) {
	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);

	contour.AddSegment(std::make_shared<Segment>(pt1, pt2));
	contour.AddSegment(std::make_shared<Segment>(pt2, pt1));

	LONGS_EQUAL(2, contour.GetSegments().size());
	CHECK(!contour.IsClosed());
}

TEST_GROUP(Contour_Triangle) {
	Contour contour;
	Point pt1, pt2, pt3;

	void setup() {
		pt1 = Point(0, 0);
		pt2 = Point(100, 0);
		pt3 = Point(50, 100);

		contour.AddSegment(std::make_shared<Segment>(pt1, pt2));
		contour.AddSegment(std::make_shared<Segment>(pt2, pt3));
		contour.AddSegment(std::make_shared<Segment>(pt3, pt1));
	}
};

TEST(Contour_Triangle, IsClosed) {
	CHECK(contour.IsClosed());
}

TEST(Contour_Triangle, IsCircle) {
	CHECK(!contour.IsCircle());
}

TEST(Contour_Triangle, OpenEnd) {
	Point pt4 = Point(5, 5);

	contour.AddSegment(std::make_shared<Segment>(pt3, pt4));

	CHECK(!contour.IsClosed());
}

TEST(Contour_Triangle, Translate) {
	Point offset(-10, 20);
	contour.Translate(offset);

	CHECK_EQUAL(pt1 + offset, contour.GetSegments()[0]->GetStart());
	CHECK_EQUAL(pt2 + offset, contour.GetSegments()[1]->GetStart());
	CHECK_EQUAL(pt3 + offset, contour.GetSegments()[2]->GetStart());
}

TEST(Contour_Triangle, Transform) {
	Transform transform(Mirroring::X, 45.0, 0.8);
	pt1.ApplyTransform(transform);
	pt2.ApplyTransform(transform);
	pt3.ApplyTransform(transform);

	contour.Transform(transform);

	CHECK_EQUAL(pt1, contour.GetSegments()[0]->GetStart());
	CHECK_EQUAL(pt2, contour.GetSegments()[1]->GetStart());
	CHECK_EQUAL(pt3, contour.GetSegments()[2]->GetStart());
}

TEST(Contour_Triangle, DeepCopy) {
	Point offset(-10, 20);

	Contour copy = contour;
	copy.Translate(offset);

	CHECK_EQUAL(pt1 + offset, copy.GetSegments()[0]->GetStart());
	CHECK_EQUAL(pt1, contour.GetSegments()[0]->GetStart());
}

TEST_GROUP(Contour_Circle) {
	Contour contour;

	void setup() {
		contour.AddSegment(
				std::make_shared<ArcSegment>(Point(), Point(), Point(100, 0),
						ArcDirection::Clockwise));
	}
};

TEST(Contour_Circle, IsCircle) {
	CHECK(contour.IsCircle());
}

TEST(Contour_Circle, IsClosed) {
	CHECK(contour.IsClosed());
}

