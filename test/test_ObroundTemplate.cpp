/*
 * test_ObroundTemplate.cpp
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

#include "ObroundTemplate.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

TEST_GROUP(ObroundTemplateTest) {
	ObroundTemplate tmp;
};

TEST(ObroundTemplateTest, TooFewParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call(1, nullptr));
}

TEST(ObroundTemplateTest, TooManyParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call(4, nullptr));
}

TEST(ObroundTemplateTest, AllParams) {
	double params[] = { 1.0, 0.5, 0.25 };
	std::shared_ptr<Aperture> aperture = tmp.Call(3, params);
	std::shared_ptr<Obround> obround = std::dynamic_pointer_cast<Obround>(aperture);

	CHECK(nullptr != obround);
	CHECK(params[0] == obround->GetXSize());
	CHECK(params[1] == obround->GetYSize());
	CHECK(params[2] == obround->GetHoleDiameter());
}

TEST(ObroundTemplateTest, DefaultHole) {
	double params[] = { 1.0, 0.5 };
	std::shared_ptr<Aperture> aperture = tmp.Call(2, params);
	std::shared_ptr<Obround> obround = std::dynamic_pointer_cast<Obround>(aperture);

	CHECK(nullptr != obround);
	CHECK(params[0] == obround->GetXSize());
	CHECK(params[1] == obround->GetYSize());
	CHECK(0.0 == obround->GetHoleDiameter());
}
