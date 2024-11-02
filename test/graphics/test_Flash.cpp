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

#include "MockAperture.h"
#include "Flash.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "GraphicsTestHelpers.h"

using namespace gerbex;

TEST_GROUP(FlashTest) {
	Point origin;
	std::shared_ptr<MockAperture> aperture;
	Flash flash;

	void setup() {
		origin = Point(2.5, -1.5);
		aperture = std::make_shared<MockAperture>();

		flash = Flash(origin, aperture);
	}
};

TEST(FlashTest, Origin) {
	CHECK_EQUAL(origin, flash.GetOrigin());
}

TEST(FlashTest, Aperture) {
	CHECK(aperture == flash.GetAperture());
}

TEST(FlashTest, Box) {
	Box box;
	Box expected = box.Translate(origin);
	mock().expectOneCall("ApertureGetBox").andReturnValue(&box);

	Box actual = flash.GetBox();

	CHECK_EQUAL(expected, actual);
}

TEST(FlashTest, Transform) {
	Transform transform(Mirroring::X, 30.0, 0.5);
	Point expectedOrigin = origin;
	expectedOrigin.ApplyTransform(transform);
	mock().expectOneCall("ApertureApplyTransform").withParameterOfType(
			"Transform", "transform", &transform);

	flash.ApplyTransform(transform);

	CHECK_EQUAL(expectedOrigin, flash.GetOrigin());
}

TEST(FlashTest, Translate) {
	Point offset(50.0, 25.0);
	Point expectedOrigin = origin + offset;

	flash.Translate(offset);

	CHECK_EQUAL(expectedOrigin, flash.GetOrigin());
}

//TODO test flash serialize
//TODO test flash toggle block aperture polarities
