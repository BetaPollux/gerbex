/*
 * test_PolygonTemplate.cpp
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

#include "GraphicsStringFrom.h"
#include "PolygonTemplate.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

#define DBL_TOL	1e-5

TEST_GROUP(PolygonTemplateTest) {
	PolygonTemplate tmp;
};

TEST(PolygonTemplateTest, TooFewParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call({ 1.0 }));
}

TEST(PolygonTemplateTest, TooManyParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call({ 1.0, 2.0, 3.0, 4.0, 5.0 }));
}

TEST(PolygonTemplateTest, AllParams) {
	Parameters params = { 1.0, 3, 45.0, 0.25 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Polygon> poly = std::dynamic_pointer_cast<Polygon>(aperture);

	CHECK(nullptr != poly);
	CHECK_EQUAL(Polygon(1.0, 3, 45.0, 0.25).GetVertices(), poly->GetVertices());
	DOUBLES_EQUAL(0.25, poly->GetHoleDiameter(), DBL_TOL);
}

TEST(PolygonTemplateTest, DefaultHole) {
	Parameters params = { 1.0, 3, 45.0 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Polygon> poly = std::dynamic_pointer_cast<Polygon>(aperture);

	CHECK(nullptr != poly);
	DOUBLES_EQUAL(0.0, poly->GetHoleDiameter(), DBL_TOL);
}

TEST(PolygonTemplateTest, NoRotation) {
	Parameters params = { 1.0, 3 };
	std::shared_ptr<Aperture> aperture = tmp.Call(params);
	std::shared_ptr<Polygon> poly = std::dynamic_pointer_cast<Polygon>(aperture);

	CHECK(nullptr != poly);
	CHECK_EQUAL(Polygon(1.0, 3).GetVertices(), poly->GetVertices());
	DOUBLES_EQUAL(0.0, poly->GetHoleDiameter(), DBL_TOL);
}

