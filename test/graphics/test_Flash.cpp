/*
 * test_Flash.cpp
 *
 *  Created on: Mar. 24, 2024
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
#include "CppUTest/TestHarness.h"

using namespace gerbex;


TEST_GROUP(FlashTest) {
	Point origin;
	std::shared_ptr<Circle> aperture;
	ApertureTransformation transform;
	Flash flash;

	void setup() {
		origin = Point(2.5, -1.5);
		aperture = std::make_shared<Circle>(2.0);
		transform.SetRotationDegrees(0.0);
		transform.SetPolarity(Polarity::Clear);

		flash = Flash(origin, aperture, transform);
	}

};

TEST(FlashTest, Origin) {
	CHECK_EQUAL(origin, flash.GetOrigin());
}

TEST(FlashTest, Aperture) {
	CHECK(aperture == flash.GetAperture());
}

TEST(FlashTest, Transform) {
	CHECK(transform == flash.GetTransform());
}

TEST(FlashTest, Box) {
	//TODO add test with rotation
	Box expected(2.0, 2.0, 1.5, -2.5);
	CHECK_EQUAL(expected, flash.GetBox());
}


