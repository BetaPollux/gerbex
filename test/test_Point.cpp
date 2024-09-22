/*
 * test_Point.cpp
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

#include "Point.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;


TEST_GROUP(PointTest) {
};

TEST(PointTest, Equal) {
	Point pt1(100, -300);
	Point pt2(100, -300);
	Point pt3(200, -300);
	Point pt4(100, -200);
	Point pt5(200, -200);

	CHECK(pt1 == pt2);
	CHECK(!(pt1 == pt3));
	CHECK(!(pt1 == pt4));
	CHECK(!(pt1 == pt5));
}

TEST(PointTest, NotEqual) {
	Point pt1(100, -300);
	Point pt2(100, -300);
	Point pt3(200, -300);
	Point pt4(100, -200);
	Point pt5(200, -200);

	CHECK(!(pt1 != pt2));
	CHECK(pt1 != pt3);
	CHECK(pt1 != pt4);
	CHECK(pt1 != pt5);
}

TEST(PointTest, Add) {
	Point pt1(100, -300);
	Point pt2(100, -300);

	CHECK((pt1 + pt2) == Point(200, -600));
}

TEST(PointTest, Subtract) {
	Point pt1(100, -300);
	Point pt2(100, -300);

	CHECK((pt1 - pt2) == Point(0, 0));
}

TEST(PointTest, Scale) {
	Point pt1(100, -300);

	CHECK((pt1 * 3) == Point(300, -900));
}


TEST_GROUP(RealPointTest) {
};

TEST(RealPointTest, Add) {
	RealPoint pt1(1.00, -3.00);
	RealPoint pt2(1.00, -3.00);

	CHECK((pt1 + pt2) == RealPoint(2.00, -6.00));
}

TEST(RealPointTest, Subtract) {
	RealPoint pt1(1.00, -3.00);
	RealPoint pt2(1.00, -3.00);

	CHECK((pt1 - pt2) == RealPoint(0.0, 0.0));
}

TEST(RealPointTest, Scale) {
	RealPoint pt1(1.00, -3.00);

	CHECK((pt1 * 2.5) == RealPoint(2.50, -7.50));
}

