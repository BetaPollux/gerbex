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

#include "CppUTest/TestHarness.h"

#include "../src/CoordinateData.h"

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
	CoordinateData xyij(0, 0, Point(0, 0));
	CoordinateData ij(std::nullopt, std::nullopt, Point(0, 0));
	CHECK(!xy.HasOffset());
	CHECK(xyij.HasOffset());
	CHECK(ij.HasOffset());
}

TEST(CoordinateDataTest, FromString_None) {
	CoordinateData coord = CoordinateData::FromString("D02");
	CHECK(!coord.HasXY());
}

TEST(CoordinateDataTest, FromString_XY) {
	CoordinateData coord = CoordinateData::FromString("X2152000Y1215000D02");
	LONGS_EQUAL(2152000, coord.GetX().value());
	LONGS_EQUAL(1215000, coord.GetY().value());
}

TEST(CoordinateDataTest, FromString_X) {
	CoordinateData coord = CoordinateData::FromString("X2152000D02");
	LONGS_EQUAL(2152000, coord.GetX().value());
}

TEST(CoordinateDataTest, FromString_Y) {
	CoordinateData coord = CoordinateData::FromString("Y1215000D02");
	LONGS_EQUAL(1215000, coord.GetY().value());
}

TEST(CoordinateDataTest, FromString_IJ) {
	CoordinateData coord = CoordinateData::FromString("I3000J0D01");
	CHECK(coord.HasOffset());
	LONGS_EQUAL(3000, coord.GetOffset().value().GetX());
	LONGS_EQUAL(0, coord.GetOffset().value().GetY());
}

TEST(CoordinateDataTest, FromString_I) {
	CoordinateData coord = CoordinateData::FromString("I3000D01");
	CHECK(!coord.HasOffset());
}

TEST(CoordinateDataTest, FromString_J) {
	CoordinateData coord = CoordinateData::FromString("J0D01");
	CHECK(!coord.HasOffset());
}

TEST(CoordinateDataTest, FromString_XYIJ) {
	CoordinateData coord = CoordinateData::FromString(
			"X5005000Y3506000I3000J0D01");
	CHECK(coord.HasXY());
	CHECK(coord.HasOffset());
	LONGS_EQUAL(5005000, coord.GetX().value());
	LONGS_EQUAL(3506000, coord.GetY().value());
	LONGS_EQUAL(3000, coord.GetOffset().value().GetX());
	LONGS_EQUAL(0, coord.GetOffset().value().GetY());
}

TEST_GROUP(CoordinateData_FromDefaults) {
	Point defaultPt;

	void setup() {
		defaultPt = Point(1000, 2500);
	}
};

TEST(CoordinateData_FromDefaults, NoData) {
	CoordinateData newData(std::nullopt, std::nullopt);
	std::optional<Point> result = CoordinateData::FromDefaults(newData, defaultPt);
	CHECK(result.value() == defaultPt);
}

TEST(CoordinateData_FromDefaults, JustX) {
	CoordinateData newData(125, std::nullopt);
	std::optional<Point> result = CoordinateData::FromDefaults(newData, defaultPt);
	CHECK(result.value() == Point(125, 2500));
}

TEST(CoordinateData_FromDefaults, JustY) {
	CoordinateData newData(std::nullopt, -500);
	std::optional<Point> result = CoordinateData::FromDefaults(newData, defaultPt);
	CHECK(result.value() == Point(1000, -500));
}

TEST(CoordinateData_FromDefaults, NoDefault) {
	CoordinateData newData(125, 500);
	std::optional<Point> result = CoordinateData::FromDefaults(newData);
	CHECK(result.value() == Point(125, 500));
}

TEST(CoordinateData_FromDefaults, MissingX) {
	CoordinateData newData(std::nullopt, 500);
	std::optional<Point> result = CoordinateData::FromDefaults(newData);
	CHECK(!result.has_value());
}

TEST(CoordinateData_FromDefaults, MissingY) {
	CoordinateData newData(125, std::nullopt);
	std::optional<Point> result = CoordinateData::FromDefaults(newData);
	CHECK(!result.has_value());
}

TEST(CoordinateData_FromDefaults, MissingXY) {
	CoordinateData newData(std::nullopt, std::nullopt);
	std::optional<Point> result = CoordinateData::FromDefaults(newData);
	CHECK(!result.has_value());
}
