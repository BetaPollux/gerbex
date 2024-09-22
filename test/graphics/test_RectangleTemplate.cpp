/*
 * test_RectangleTemplate.cpp
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

#include "RectangleTemplate.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;


TEST_GROUP(RectangleTemplateTest) {
	RectangleTemplate tmp;
};

TEST(RectangleTemplateTest, TooFewParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call({ 1.0 }));
}

TEST(RectangleTemplateTest, TooManyParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call({ 1.0, 2.0, 3.0, 4.0 }));
}

TEST(RectangleTemplateTest, AllParams) {
	std::vector<double> params = { 1.0, 0.5, 0.25 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Rectangle> rect = std::dynamic_pointer_cast<Rectangle>(aperture);

	CHECK(nullptr != rect);
	CHECK(params[0] == rect->GetXSize());
	CHECK(params[1] == rect->GetYSize());
	CHECK(params[2] == rect->GetHoleDiameter());
}

TEST(RectangleTemplateTest, DefaultHole) {
	std::vector<double> params = { 1.0, 0.5 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Rectangle> rect = std::dynamic_pointer_cast<Rectangle>(aperture);

	CHECK(nullptr != rect);
	CHECK(params[0] == rect->GetXSize());
	CHECK(params[1] == rect->GetYSize());
	CHECK(0.0 == rect->GetHoleDiameter());
}
