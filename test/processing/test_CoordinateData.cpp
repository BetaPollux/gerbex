/*
 * test_CoordinateData.cpp
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

#include "CoordinateData.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"


using namespace gerbex;

TEST_GROUP(CoordinateDataTest) {
};

TEST(CoordinateDataTest, HasXY) {
	CoordinateData none(std::nullopt, std::nullopt);
	CoordinateData just_x(0, std::nullopt);
	CoordinateData just_y(std::nullopt, 0);
	CoordinateData both(0, 0);

	CHECK(!none.HasXY());
	CHECK(!just_x.HasXY());
	CHECK(!just_y.HasXY());
	CHECK(both.HasXY());
}

TEST(CoordinateDataTest, HasOffset) {
	CoordinateData xy(0, 0, std::nullopt);
	CoordinateData xyij(0, 0, FixedPoint(0, 0));
	CoordinateData ij(std::nullopt, std::nullopt, FixedPoint(0, 0));
	CHECK(!xy.GetIJ().has_value());
	CHECK(xyij.GetIJ().has_value());
	CHECK(ij.GetIJ().has_value());
}

TEST(CoordinateDataTest, FromString_None) {
	CoordinateData coord = CoordinateData::FromString("D02");
	CHECK(!coord.HasXY());
}

TEST(CoordinateDataTest, FromString_XY) {
	CoordinateData coord = CoordinateData::FromString("X2152000Y1215000D02");
	LONGS_EQUAL(2152000, *coord.GetX());
	LONGS_EQUAL(1215000, *coord.GetY());
}

TEST(CoordinateDataTest, FromString_X) {
	CoordinateData coord = CoordinateData::FromString("X2152000D02");
	LONGS_EQUAL(2152000, *coord.GetX());
}

TEST(CoordinateDataTest, FromString_Y) {
	CoordinateData coord = CoordinateData::FromString("Y1215000D02");
	LONGS_EQUAL(1215000, *coord.GetY());
}

TEST(CoordinateDataTest, FromString_IJ) {
	CoordinateData coord = CoordinateData::FromString("I3000J0D01");
	CHECK(coord.GetIJ().has_value());
	LONGS_EQUAL(3000, coord.GetIJ()->GetX());
	LONGS_EQUAL(0, coord.GetIJ()->GetY());
}

TEST(CoordinateDataTest, FromString_I) {
	CoordinateData coord = CoordinateData::FromString("I3000D01");
	CHECK(!coord.GetIJ().has_value());
}

TEST(CoordinateDataTest, FromString_J) {
	CoordinateData coord = CoordinateData::FromString("J0D01");
	CHECK(!coord.GetIJ().has_value());
}

TEST(CoordinateDataTest, FromString_XYIJ) {
	CoordinateData coord = CoordinateData::FromString(
			"X5005000Y3506000I3000J0D01");
	CHECK(coord.HasXY());
	LONGS_EQUAL(5005000, *coord.GetX());
	LONGS_EQUAL(3506000, *coord.GetY());
	LONGS_EQUAL(3000, coord.GetIJ()->GetX());
	LONGS_EQUAL(0, coord.GetIJ()->GetY());
}

TEST(CoordinateDataTest, FromString_XYIJ_WithSign) {
	CoordinateData coord = CoordinateData::FromString(
			"X+5005000Y-3506000I-3000J+0D01");
	CHECK(coord.HasXY());
	LONGS_EQUAL(5005000, *coord.GetX());
	LONGS_EQUAL(-3506000, *coord.GetY());
	LONGS_EQUAL(-3000, coord.GetIJ()->GetX());
	LONGS_EQUAL(0, coord.GetIJ()->GetY());
}

