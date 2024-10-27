/*
 * test_CircleTemplate.cpp
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

#include "CircleTemplate.h"
#include "GraphicsStringFrom.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP(CircleTemplateTest) {
	CircleTemplate tmp;
};

TEST(CircleTemplateTest, TooFewParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call( { }));
}

TEST(CircleTemplateTest, TooManyParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call( { 1.0, 2.0, 3.0 }));
}

TEST(CircleTemplateTest, AllParams) {
	Parameters params = { 1.0, 0.25 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(
			aperture);

	Circle expected(1.0, 0.25);
	CHECK(nullptr != circle);
	CHECK_EQUAL(expected, *circle);
}

TEST(CircleTemplateTest, DefaultHole) {
	Parameters params = { 1.0 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(
			aperture);

	Circle expected(1.0);
	CHECK(nullptr != circle);
	CHECK_EQUAL(expected, *circle);
}

