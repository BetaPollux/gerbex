/*
 * test_Coordinate.cpp
 *
 *  Created on: Sep. 15, 2024
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

#include "CppUTest/TestHarness.h"
#include "Coordinate.h"

TEST_GROUP(CoordinateTest) {
};

TEST(CoordinateTest, HasXY) {
	Coordinate none(std::nullopt, std::nullopt);
	Coordinate just_x(0, std::nullopt);
	Coordinate just_y(std::nullopt, 0);
	Coordinate both(0, 0);

	CHECK(!none.HasXY());
	CHECK(!just_x.HasXY());
	CHECK(!just_y.HasXY());
	CHECK(both.HasXY());
}

TEST(CoordinateTest, HasOffset) {
	Coordinate xy(0, 0, std::nullopt);
	Coordinate xyij(0, 0, Point(0, 0));
	Coordinate ij(std::nullopt, std::nullopt, Point(0, 0));
	CHECK(!xy.HasOffset());
	CHECK(xyij.HasOffset());
	CHECK(ij.HasOffset());
}

TEST(CoordinateTest, FromString_None) {
	Coordinate coord = Coordinate::FromString("D02");
	CHECK(!coord.HasXY());
}

TEST(CoordinateTest, FromString_XY) {
	Coordinate coord = Coordinate::FromString("X2152000Y1215000D02");
	LONGS_EQUAL(2152000, coord.GetX().value());
	LONGS_EQUAL(1215000, coord.GetY().value());
}

TEST(CoordinateTest, FromString_X) {
	Coordinate coord = Coordinate::FromString("X2152000D02");
	LONGS_EQUAL(2152000, coord.GetX().value());
}

TEST(CoordinateTest, FromString_Y) {
	Coordinate coord = Coordinate::FromString("Y1215000D02");
	LONGS_EQUAL(1215000, coord.GetY().value());
}

TEST(CoordinateTest, FromString_IJ) {
	Coordinate coord = Coordinate::FromString("I3000J0D01");
	CHECK(coord.HasOffset());
	LONGS_EQUAL(3000, coord.GetOffset().value().GetX());
	LONGS_EQUAL(0, coord.GetOffset().value().GetY());
}

TEST(CoordinateTest, FromString_I) {
	Coordinate coord = Coordinate::FromString("I3000D01");
	CHECK(!coord.HasOffset());
}

TEST(CoordinateTest, FromString_J) {
	Coordinate coord = Coordinate::FromString("J0D01");
	CHECK(!coord.HasOffset());
}

TEST(CoordinateTest, FromString_XYIJ) {
	Coordinate coord = Coordinate::FromString("X5005000Y3506000I3000J0D01");
	CHECK(coord.HasXY());
	CHECK(coord.HasOffset());
	LONGS_EQUAL(5005000, coord.GetX().value());
	LONGS_EQUAL(3506000, coord.GetY().value());
	LONGS_EQUAL(3000, coord.GetOffset().value().GetX());
	LONGS_EQUAL(0, coord.GetOffset().value().GetY());
}
