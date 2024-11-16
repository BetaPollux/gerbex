/*
 * test_Region.cpp
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

#include "ArcSegment.h"
#include "Region.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"
#include "GraphicsTestHelpers.h"

using namespace gerbex;



/***
 * From clean initialization
 */
TEST_GROUP(Region_Init) {
	Region region;
};

TEST(Region_Init, ContoursSize) {
	LONGS_EQUAL(0, region.GetContours().size());
}

TEST(Region_Init, AddSegment_NoContours) {
	std::shared_ptr<Segment> segment = std::make_shared<Segment>();

	CHECK_THROWS(std::logic_error, region.AddSegment(segment));
}

TEST(Region_Init, Closed) {
	CHECK(!region.AreContoursClosed());
}

/***
 * Region with one contour
 */
TEST_GROUP(Region_OneContour) {
	Region region;

	void setup() {
		region.StartContour();
	}
};

TEST(Region_OneContour, ContourSize) {
	LONGS_EQUAL(1, region.GetContours().size());
}

TEST(Region_OneContour, AddSegment) {
	std::shared_ptr<Segment> segment = std::make_shared<Segment>(Point(0, 0), Point(0, 100));

	region.AddSegment(segment);

	auto segments = region.GetContours().back().GetSegments();

	LONGS_EQUAL(1, segments.size());
	POINTERS_EQUAL(segment.get(), segments.back().get());
}

TEST(Region_OneContour, AddMultiSegments) {
	std::shared_ptr<Segment> segment = std::make_shared<Segment>(Point(0, 0), Point(0, 100));
	std::shared_ptr<ArcSegment> arcSegment = std::make_shared<ArcSegment>(Point(0, 100), Point(100, 0),
			Point(0, 0), ArcDirection::Clockwise);
	std::shared_ptr<Segment> segment2 = std::make_shared<Segment>(Point(100, 0), Point(0, 0));

	region.AddSegment(segment);
	region.AddSegment(arcSegment);
	region.AddSegment(segment2);

	auto segments = region.GetContours().back().GetSegments();

	LONGS_EQUAL(3, segments.size());
	POINTERS_EQUAL(segment.get(), segments[0].get());
	POINTERS_EQUAL(arcSegment.get(), segments[1].get());
	POINTERS_EQUAL(segment2.get(), segments[2].get());
}

TEST(Region_OneContour, StartNextContour_NotClosed) {
	std::shared_ptr<Segment> segment = std::make_shared<Segment>(Point(0, 0), Point(0, 100));

	//Open contour
	region.AddSegment(segment);

	CHECK_THROWS(std::logic_error, region.StartContour());
}

/***
 * Region with multiple contours
 */
TEST_GROUP(Region_MultiContour) {
	Region region;

	void setup() {
		region.StartContour();
		region.AddSegment(std::make_shared<Segment>(Point(0, 0), Point(0, 100)));
		region.AddSegment(std::make_shared<Segment>(Point(0, 100), Point(100, 0)));
		region.AddSegment(std::make_shared<Segment>(Point(100, 0), Point(0, 0)));
		region.StartContour();
	}
};

TEST(Region_MultiContour, ContourSize) {
	LONGS_EQUAL(2, region.GetContours().size());
}

TEST(Region_MultiContour, AddSegment) {
	std::shared_ptr<Segment> segment = std::make_shared<Segment>(Point(0, 0), Point(0, 100));

	int segments0_size = region.GetContours()[0].GetSegments().size();
	region.AddSegment(segment);

	auto segments0 = region.GetContours()[0].GetSegments();
	auto segments1 = region.GetContours()[1].GetSegments();

	LONGS_EQUAL(segments0_size, segments0.size());
	LONGS_EQUAL(1, segments1.size());
	POINTERS_EQUAL(segment.get(), segments1.back().get());
}

/***
 * Get box for region
 */
TEST_GROUP(Region_Box) {
	Region region;

	void setup() {
		region.StartContour();
		region.AddSegment(std::make_shared<Segment>(Point(0, 0), Point(0, 100)));
		region.AddSegment(std::make_shared<Segment>(Point(0, 100), Point(100, 0)));
		region.AddSegment(std::make_shared<Segment>(Point(100, 0), Point(0, 0)));
	}
};

TEST(Region_Box, Single) {
	Box expected(100.0, 100.0, 0.0, 0.0);
	CHECK_EQUAL(expected, region.GetBox());
}

TEST(Region_Box, Open) {
	region.StartContour();
	CHECK_THROWS(std::invalid_argument, region.GetBox());
}

TEST(Region_Box, Multi) {
	Box expected(200.0, 200.0, -100.0, -100.0);

	region.StartContour();
	region.AddSegment(std::make_shared<Segment>(Point(-100, -100), Point(-100, 0)));
	region.AddSegment(std::make_shared<Segment>(Point(-100, 0), Point(0, -100)));
	region.AddSegment(std::make_shared<Segment>(Point(0, -100), Point(-100, -100)));

	CHECK_EQUAL(expected, region.GetBox());
}

//TODO test region serialize, transform, translate, clone
//TODO test box that excludes 0, 0
