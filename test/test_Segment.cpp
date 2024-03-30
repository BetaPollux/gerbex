/*
 * test_Segment.cpp
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
#include "Segment.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(Segment) {
};

TEST(Segment, DefaultCtor) {
	Segment segment;

	CHECK(nullptr == segment.GetAperture());
}

TEST(Segment, PointsCtor) {
	Point origin(2500, 5000);
	Point endPoint(7500, 1500);

	Segment segment(origin, endPoint);

	CHECK(origin == segment.GetOrigin());
	CHECK(endPoint == segment.GetEndPoint());
	CHECK(nullptr == segment.GetAperture());
}

TEST(Segment, FullCtor) {
	Point origin(2500, 5000);
	Point endPoint(7500, 1500);
	std::shared_ptr<Circle> circle = std::make_shared<Circle>();
	ApertureTransformation transform(LoadPolarity::Clear, LoadMirroring::XY, 45.0, 1.5);

	Segment segment(origin, endPoint, circle, transform);

	CHECK(origin == segment.GetOrigin());
	CHECK(endPoint == segment.GetEndPoint());
	CHECK(circle == segment.GetAperture());
	CHECK(transform == segment.GetTransformation());
}

