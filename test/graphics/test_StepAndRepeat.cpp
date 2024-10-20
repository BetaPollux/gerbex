/*
 * test_StepAndRepeat.cpp
 *
 *  Created on: Mar. 30, 2024
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
#include "Flash.h"
#include "GraphicsStringFrom.h"
#include "StepAndRepeat.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(StepAndRepeatTest) {
};

TEST(StepAndRepeatTest, DefaultCtor) {
	StepAndRepeat sr;
	CHECK(sr.GetNx() == 1);
	CHECK(sr.GetNy() == 1);
	CHECK(sr.GetDx() == 0.0);
	CHECK(sr.GetDy() == 0.0);
}

TEST(StepAndRepeatTest, InvalidX) {
	CHECK_THROWS(std::invalid_argument, StepAndRepeat(0, 1, 0.0, 0.0));
	CHECK_THROWS(std::invalid_argument, StepAndRepeat(-1, 1, 0.0, 0.0));
}

TEST(StepAndRepeatTest, InvalidY) {
	CHECK_THROWS(std::invalid_argument, StepAndRepeat(1, 0, 0.0, 0.0));
	CHECK_THROWS(std::invalid_argument, StepAndRepeat(1, -1, 0.0, 0.0));
}

TEST(StepAndRepeatTest, Invalid_dX) {
	CHECK_THROWS(std::invalid_argument, StepAndRepeat(1, 1, -1.0, 0.0));
}

TEST(StepAndRepeatTest, Invalid_dY) {
	CHECK_THROWS(std::invalid_argument, StepAndRepeat(1, 1, 0.0, -1.0));
}

TEST(StepAndRepeatTest, GetObjectList) {
	StepAndRepeat sr;
	CHECK(nullptr != sr.GetObjectList());
}

TEST(StepAndRepeatTest, AddObject) {
	StepAndRepeat sr;
	std::shared_ptr<Flash> flash = std::make_shared<Flash>();

	sr.AddObject(flash);
	LONGS_EQUAL(1, sr.GetObjectList()->size());
}

TEST(StepAndRepeatTest, GetBox) {
	double radius = 1.0;
	std::shared_ptr<Circle> circle = std::make_shared<Circle>(2 * radius);
	Point origin(5.0, 5.0);
	ApertureTransformation transform;
	std::shared_ptr<Flash> flash = std::make_shared<Flash>(origin, circle,
			transform);

	StepAndRepeat sr(3, 2, 5.0, 4.0);
	sr.AddObject(flash);

	double left = origin.GetX() - radius;
	double right = origin.GetX() + (sr.GetNx() - 1) * sr.GetDx() + radius;
	double bottom = origin.GetY() - radius;
	double top = origin.GetY() + (sr.GetNy() - 1) * sr.GetDy() + radius;
	Box expected(right - left, top - bottom, left, bottom);

	CHECK_EQUAL(expected, sr.GetBox());
}

TEST(StepAndRepeatTest, GetBox_Empty) {
	StepAndRepeat sr(3, 2, 5.0, 5.0);

	CHECK_THROWS(std::invalid_argument, sr.GetBox());
}
