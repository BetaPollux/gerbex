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

#include "PolygonTemplate.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

TEST_GROUP(PolygonTemplateTest) {
	PolygonTemplate tmp;
};

TEST(PolygonTemplateTest, TooFewParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call(1, nullptr));
}

TEST(PolygonTemplateTest, TooManyParams) {
	CHECK_THROWS(std::invalid_argument, tmp.Call(5, nullptr));
}

TEST(PolygonTemplateTest, AllParams) {
	double params[] = { 1.0, 3, 45.0, 0.25 };
	std::shared_ptr<Aperture> aperture = tmp.Call(4, params);
	std::shared_ptr<Polygon> poly = std::dynamic_pointer_cast<Polygon>(aperture);

	CHECK(nullptr != poly);
	CHECK(params[0] == poly->GetOuterDiameter());
	CHECK((int)params[1] == poly->GetNumVertices());
	CHECK(params[2] == poly->GetRotation());
	CHECK(params[3] == poly->GetHoleDiameter());
}

TEST(PolygonTemplateTest, DefaultHole) {
	double params[] = { 1.0, 3, 45.0 };
	std::shared_ptr<Aperture> aperture = tmp.Call(3, params);
	std::shared_ptr<Polygon> poly = std::dynamic_pointer_cast<Polygon>(aperture);

	CHECK(nullptr != poly);
	CHECK(params[0] == poly->GetOuterDiameter());
	CHECK((int)params[1] == poly->GetNumVertices());
	CHECK(params[2] == poly->GetRotation());
	CHECK(0.0 == poly->GetHoleDiameter());
}

TEST(PolygonTemplateTest, NoRotation) {
	double params[] = { 1.0, 3 };
	std::shared_ptr<Aperture> aperture = tmp.Call(2, params);
	std::shared_ptr<Polygon> poly = std::dynamic_pointer_cast<Polygon>(aperture);

	CHECK(nullptr != poly);
	CHECK(params[0] == poly->GetOuterDiameter());
	CHECK((int)params[1] == poly->GetNumVertices());
	CHECK(0.0 == poly->GetRotation());
	CHECK(0.0 == poly->GetHoleDiameter());
}

