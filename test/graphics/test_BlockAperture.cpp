/*
 * test_BlockAperture.cpp
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

#include "BlockAperture.h"
#include "Circle.h"
#include "Flash.h"
#include "GraphicsStringFrom.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(BlockApertureTest) {
	BlockAperture block;
};

TEST(BlockApertureTest, GetObjectList) {
	CHECK(nullptr != block.GetObjectList());
}

TEST(BlockApertureTest, AddObject) {
	std::shared_ptr<Flash> flash = std::make_shared<Flash>();
	block.AddObject(flash);
	LONGS_EQUAL(1, block.GetObjectList()->size());
}

TEST(BlockApertureTest, GetBox) {
	std::shared_ptr<Circle> circle = std::make_shared<Circle>(2.0);
	Point origin1(5.0, 5.0);
	Point origin2(25.0, 15.0);
	ApertureTransformation transform;
	std::shared_ptr<Flash> flash1 = std::make_shared<Flash>(origin1, circle, transform);
	std::shared_ptr<Flash> flash2 = std::make_shared<Flash>(origin2, circle, transform);
	block.AddObject(flash1);
	block.AddObject(flash2);

	Box expected(22.0, 12.0, 4.0, 4.0);

	CHECK_EQUAL(expected, block.GetBox());
}

TEST(BlockApertureTest, GetBox_Empty) {
	CHECK_THROWS(std::invalid_argument, block.GetBox());
}
