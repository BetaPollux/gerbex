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

#include "Arc.h"
#include "Draw.h"
#include "Region.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

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
	std::shared_ptr<Draw> draw = std::make_shared<Draw>();

	CHECK_THROWS(std::logic_error, region.AddSegment(draw));
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
	std::shared_ptr<Draw> draw = std::make_shared<Draw>();

	region.AddSegment(draw);

	auto segments = region.GetContours().back()->GetSegments();

	LONGS_EQUAL(1, segments.size());
	POINTERS_EQUAL(draw.get(), segments.back().get());
}

TEST(Region_OneContour, AddMultiSegments) {
	std::shared_ptr<Draw> draw = std::make_shared<Draw>();
	std::shared_ptr<Arc> arc = std::make_shared<Arc>();
	std::shared_ptr<Draw> draw2 = std::make_shared<Draw>();

	region.AddSegment(draw);
	region.AddSegment(arc);
	region.AddSegment(draw2);

	auto segments = region.GetContours().back()->GetSegments();

	LONGS_EQUAL(3, segments.size());
	POINTERS_EQUAL(draw.get(), segments[0].get());
	POINTERS_EQUAL(arc.get(), segments[1].get());
	POINTERS_EQUAL(draw2.get(), segments[2].get());
}

/***
 * Region with multiple contours
 */
TEST_GROUP(Region_MultiContour) {
	Region region;

	void setup() {
		region.StartContour();
		region.StartContour();
	}
};

TEST(Region_MultiContour, ContourSize) {
	LONGS_EQUAL(2, region.GetContours().size());
}

TEST(Region_MultiContour, AddSegment) {
	std::shared_ptr<Draw> draw = std::make_shared<Draw>();

	region.AddSegment(draw);

	auto segments0 = region.GetContours()[0]->GetSegments();
	auto segments1 = region.GetContours()[1]->GetSegments();

	LONGS_EQUAL(0, segments0.size());
	LONGS_EQUAL(1, segments1.size());
	POINTERS_EQUAL(draw.get(), segments1.back().get());
}

