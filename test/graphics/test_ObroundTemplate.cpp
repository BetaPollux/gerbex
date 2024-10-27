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

using namespace gerbex;

#define DBL_TOL	1e-5

TEST_GROUP(ObroundTemplateTest) {
	ObroundTemplate tmp;
};

TEST(ObroundTemplateTest, TooFewParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call({ 1.0 }));
}

TEST(ObroundTemplateTest, TooManyParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call({ 1.0, 2.0, 3.0, 4.0 }));
}

TEST(ObroundTemplateTest, AllParams) {
	Parameters params = { 1.0, 0.5, 0.25 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Obround> obround = std::dynamic_pointer_cast<Obround>(aperture);

	CHECK(nullptr != obround);
	DOUBLES_EQUAL(1.0, obround->GetLength(), DBL_TOL);
	DOUBLES_EQUAL(0.5, obround->GetDrawWidth(), DBL_TOL);
	DOUBLES_EQUAL(0.25, obround->GetHoleDiameter(), DBL_TOL);
}

TEST(ObroundTemplateTest, DefaultHole) {
	Parameters params = { 1.0, 0.5 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Obround> obround = std::dynamic_pointer_cast<Obround>(aperture);

	CHECK(nullptr != obround);
	DOUBLES_EQUAL(0.0, obround->GetHoleDiameter(), DBL_TOL);
}
