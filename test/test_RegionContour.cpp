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
#include "Draw.h"
#include "RegionContour.h"
#include <memory>
#include "CppUTest/TestHarness.h"

TEST_GROUP(RegionContourTest) {
};

TEST(RegionContourTest, Initial) {
	RegionContour contour;
	LONGS_EQUAL(0, contour.GetSegments().size());
}

TEST(RegionContourTest, AddSegment) {
	RegionContour contour;
	std::shared_ptr<Draw> draw = std::make_shared<Draw>();

	contour.AddSegment(draw);

	LONGS_EQUAL(1, contour.GetSegments().size());
	POINTERS_EQUAL(draw.get(), contour.GetSegments().back().get());
}

TEST(RegionContourTest, IsClosed_Empty) {
	RegionContour contour;
	CHECK(!contour.IsClosed());
}

TEST(RegionContourTest, IsClosed_TwoSegments) {
	RegionContour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);

	contour.AddSegment(std::make_shared<Draw>(pt1, pt2));
	contour.AddSegment(std::make_shared<Draw>(pt2, pt1));

	LONGS_EQUAL(2, contour.GetSegments().size());
	CHECK(!contour.IsClosed());
}

TEST(RegionContourTest, IsClosed_Triangle) {
	RegionContour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);
	Point pt3 = Point(50, 100);

	contour.AddSegment(std::make_shared<Draw>(pt1, pt2));
	contour.AddSegment(std::make_shared<Draw>(pt2, pt3));
	contour.AddSegment(std::make_shared<Draw>(pt3, pt1));

	LONGS_EQUAL(3, contour.GetSegments().size());
	CHECK(contour.IsClosed());
}

TEST(RegionContourTest, IsClosed_Triangle_OpenEnd) {
	RegionContour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);
	Point pt3 = Point(50, 100);
	Point pt4 = Point(5, 5);

	contour.AddSegment(std::make_shared<Draw>(pt1, pt2));
	contour.AddSegment(std::make_shared<Draw>(pt2, pt3));
	contour.AddSegment(std::make_shared<Draw>(pt3, pt4));

	LONGS_EQUAL(3, contour.GetSegments().size());
	CHECK(!contour.IsClosed());
}

TEST(RegionContourTest, IsClosed_Triangle_OpenTop) {
	RegionContour contour;

	Point pt1 = Point(0, 0);
	Point pt2 = Point(100, 0);
	Point pt3 = Point(105, 0);
	Point pt4 = Point(50, 100);

	contour.AddSegment(std::make_shared<Draw>(pt1, pt2));
	contour.AddSegment(std::make_shared<Draw>(pt3, pt4));
	contour.AddSegment(std::make_shared<Draw>(pt4, pt1));

	LONGS_EQUAL(3, contour.GetSegments().size());
	CHECK(!contour.IsClosed());
}

