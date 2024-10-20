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
#include "GraphicsStringFrom.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

#define DBL_TOL	1e-9

using namespace gerbex;

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

TEST(ApertureTransformationTest, ApplyScaling) {
	transform.SetScalingFactor(0.8);
	double result = transform.ApplyScaling(5.0);

	CHECK_EQUAL(4.0, result);
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

TEST(ApertureTransformationTest, RejectsScaling) {
	CHECK_THROWS(std::invalid_argument, transform.SetScalingFactor(0.0));
	CHECK_THROWS(std::invalid_argument, transform.SetScalingFactor(-1.0));
}

TEST(ApertureTransformationTest, PolarityFromCommand) {
	CHECK(Polarity::Clear == ApertureTransformation::PolarityFromCommand("C"));
	CHECK(Polarity::Dark == ApertureTransformation::PolarityFromCommand("D"));
	CHECK_THROWS(std::invalid_argument,
			ApertureTransformation::PolarityFromCommand("Q"));
}

TEST(ApertureTransformationTest, MirroringFromCommand) {
	CHECK(Mirroring::None == ApertureTransformation::MirroringFromCommand("N"));
	CHECK(Mirroring::X == ApertureTransformation::MirroringFromCommand("X"));
	CHECK(Mirroring::Y == ApertureTransformation::MirroringFromCommand("Y"));
	CHECK(Mirroring::XY == ApertureTransformation::MirroringFromCommand("XY"));
	CHECK_THROWS(std::invalid_argument,
			ApertureTransformation::MirroringFromCommand("Q"));
}

TEST_GROUP(ApertureTransformation_Apply) {
	ApertureTransformation transform;
	Point point;

	void setup() {
		point = Point(1.0, 1.0);
	}
};

TEST(ApertureTransformation_Apply, Neutral) {
	Point result = transform.Apply(point);

	CHECK_EQUAL(point, result);
}

TEST(ApertureTransformation_Apply, Scaling) {
	transform.SetScalingFactor(0.6);
	Point expected(0.6, 0.6);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, MirrorX) {
	transform.SetMirroring(Mirroring::X);
	Point expected(-1.0, 1.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, MirrorY) {
	transform.SetMirroring(Mirroring::Y);
	Point expected(1.0, -1.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, MirrorXY) {
	transform.SetMirroring(Mirroring::XY);
	Point expected(-1.0, -1.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, RotateCCW) {
	transform.SetRotationDegrees(45.0);
	Point expected(0.0, sqrt(2.0));
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, RotateCW) {
	transform.SetRotationDegrees(-45.0);
	Point expected(sqrt(2.0), 0.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, MirrorXThenRotate) {
	transform.SetMirroring(Mirroring::X);
	transform.SetRotationDegrees(45.0);
	Point expected(-sqrt(2.0), 0.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, MirrorYThenRotate) {
	transform.SetMirroring(Mirroring::Y);
	transform.SetRotationDegrees(45.0);
	Point expected(sqrt(2.0), 0.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST_GROUP(ApertureTransformation_Stack) {
	ApertureTransformation transform;
	ApertureTransformation other;

	void setup() {
		transform = ApertureTransformation();
		other = ApertureTransformation();
	}
};

TEST(ApertureTransformation_Stack, Scaling) {
	transform.SetScalingFactor(0.8);
	other.SetScalingFactor(0.5);
	ApertureTransformation result = transform.Stack(other);

	DOUBLES_EQUAL(0.4, result.GetScalingFactor(), DBL_TOL);
}

TEST(ApertureTransformation_Stack, Rotation) {
	transform.SetRotationDegrees(30.0);
	other.SetRotationDegrees(-10.0);
	ApertureTransformation result = transform.Stack(other);

	DOUBLES_EQUAL(20.0, result.GetRotationDegrees(), DBL_TOL);
}

TEST(ApertureTransformation_Stack, PolarityClearIgnoresDark) {
	transform.SetPolarity(Polarity::Clear);
	other.SetPolarity(Polarity::Dark);
	ApertureTransformation result = transform.Stack(other);

	CHECK(Polarity::Clear == result.GetPolarity());
}

TEST(ApertureTransformation_Stack, PolarityDarkToClear) {
	transform.SetPolarity(Polarity::Dark);
	other.SetPolarity(Polarity::Clear);
	ApertureTransformation result = transform.Stack(other);

	CHECK(Polarity::Clear == result.GetPolarity());
}

TEST(ApertureTransformation_Stack, PolarityClearToDark) {
	transform.SetPolarity(Polarity::Clear);
	other.SetPolarity(Polarity::Clear);
	ApertureTransformation result = transform.Stack(other);

	CHECK(Polarity::Dark == result.GetPolarity());
}

TEST(ApertureTransformation_Stack, MirrorNoneToX) {
	transform.SetMirroring(Mirroring::None);
	other.SetMirroring(Mirroring::X);
	ApertureTransformation result = transform.Stack(other);

	CHECK(Mirroring::X == result.GetMirroring());
}

TEST(ApertureTransformation_Stack, MirrorXToNone) {
	transform.SetMirroring(Mirroring::X);
	other.SetMirroring(Mirroring::X);
	ApertureTransformation result = transform.Stack(other);

	CHECK(Mirroring::None == result.GetMirroring());
}

TEST(ApertureTransformation_Stack, MirrorNoneToY) {
	transform.SetMirroring(Mirroring::None);
	other.SetMirroring(Mirroring::Y);
	ApertureTransformation result = transform.Stack(other);

	CHECK(Mirroring::Y == result.GetMirroring());
}

TEST(ApertureTransformation_Stack, MirrorYToNone) {
	transform.SetMirroring(Mirroring::Y);
	other.SetMirroring(Mirroring::Y);
	ApertureTransformation result = transform.Stack(other);

	CHECK(Mirroring::None == result.GetMirroring());
}
