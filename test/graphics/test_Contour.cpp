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

#include "Circle.h"
#include "Contour.h"
#include <memory>
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;


TEST_GROUP(ContourTest) {
};

TEST(ContourTest, Initial) {
	Contour contour;
	LONGS_EQUAL(0, contour.GetSegments().size());
}

TEST(ContourTest, AddSegment) {
	Contour contour;
	std::shared_ptr<Segment> segment = std::make_shared<Segment>(Point(0, 0),
			Point(0, 100));

	contour.AddSegment(segment);

	LONGS_EQUAL(1, contour.GetSegments().size());
	POINTERS_EQUAL(segment.get(), contour.GetSegments().back().get());
}

TEST(ContourTest, AddSegment_ZeroLength) {
	Contour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(0, 0);

	CHECK_THROWS(std::invalid_argument,
			contour.AddSegment(std::make_shared<Segment>(pt1, pt2)));
}

TEST(ContourTest, IsClosed_Empty) {
	Contour contour;
	CHECK(!contour.IsClosed());
}

TEST(ContourTest, IsClosed_TwoSegments) {
	Contour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);

	contour.AddSegment(std::make_shared<Segment>(pt1, pt2));
	contour.AddSegment(std::make_shared<Segment>(pt2, pt1));

	LONGS_EQUAL(2, contour.GetSegments().size());
	CHECK(!contour.IsClosed());
}

TEST(ContourTest, IsClosed_Triangle) {
	Contour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);
	Point pt3 = Point(50, 100);

	contour.AddSegment(std::make_shared<Segment>(pt1, pt2));
	contour.AddSegment(std::make_shared<Segment>(pt2, pt3));
	contour.AddSegment(std::make_shared<Segment>(pt3, pt1));

	LONGS_EQUAL(3, contour.GetSegments().size());
	CHECK(contour.IsClosed());
}

TEST(ContourTest, IsClosed_Triangle_OpenEnd) {
	Contour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);
	Point pt3 = Point(50, 100);
	Point pt4 = Point(5, 5);

	contour.AddSegment(std::make_shared<Segment>(pt1, pt2));
	contour.AddSegment(std::make_shared<Segment>(pt2, pt3));
	contour.AddSegment(std::make_shared<Segment>(pt3, pt4));

	LONGS_EQUAL(3, contour.GetSegments().size());
	CHECK(!contour.IsClosed());
}

TEST(ContourTest, IsClosed_Triangle_OpenTop) {
	Contour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);
	Point pt3 = Point(105, 0);
	Point pt4 = Point(50, 100);

	contour.AddSegment(std::make_shared<Segment>(pt1, pt2));
	contour.AddSegment(std::make_shared<Segment>(pt3, pt4));
	contour.AddSegment(std::make_shared<Segment>(pt4, pt1));

	LONGS_EQUAL(3, contour.GetSegments().size());
	CHECK(!contour.IsClosed());
}

//TODO test translate, transform

