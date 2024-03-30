/*
 * test_ApertureTransformation.cpp
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

#include "ApertureTransformation.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(ApertureTransformationTest) {
	ApertureTransformation transform;
};

TEST(ApertureTransformationTest, Polarity) {
	CHECK(Polarity::Dark == transform.GetPolarity());
}

TEST(ApertureTransformationTest, Mirroring) {
	CHECK(Mirroring::None == transform.GetMirroring());
}

TEST(ApertureTransformationTest, Rotation) {
	CHECK(0.0 == transform.GetRotationDegrees());
}

TEST(ApertureTransformationTest, Scaling) {
	CHECK(1.0 == transform.GetScalingFactor());
}

TEST(ApertureTransformationTest, Equal) {
	ApertureTransformation other;

	CHECK(other == transform);
	CHECK(!(other != transform));
}

TEST(ApertureTransformationTest, NotEqual_Polarity) {
	ApertureTransformation other;
	other.SetPolarity(Polarity::Clear);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

TEST(ApertureTransformationTest, NotEqual_Mirroring) {
	ApertureTransformation other;
	other.SetMirroring(Mirroring::X);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

TEST(ApertureTransformationTest, NotEqual_Rotation) {
	ApertureTransformation other;
	other.SetRotationDegrees(30.0);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

TEST(ApertureTransformationTest, NotEqual_Scaling) {
	ApertureTransformation other;
	other.SetScalingFactor(2.25);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

