/*
 * test_Transform.cpp
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

#include "GraphicsStringFrom.h"
#include "Transform.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

#define DBL_TOL	1e-9

using namespace gerbex;

TEST_GROUP(TransformTest) {
	Transform transform;
};

TEST(TransformTest, Polarity) {
	CHECK(Polarity::Dark == transform.GetPolarity());
}

TEST(TransformTest, Mirroring) {
	CHECK(Mirroring::None == transform.GetMirroring());
}

TEST(TransformTest, Rotation) {
	CHECK(0.0 == transform.GetRotation());
}

TEST(TransformTest, Scaling) {
	CHECK(1.0 == transform.GetScaling());
}

TEST(TransformTest, ApplyScaling) {
	transform.SetScaling(0.8);
	double result = transform.ApplyScaling(5.0);

	CHECK_EQUAL(4.0, result);
}

TEST(TransformTest, Equal) {
	Transform other;

	CHECK(other == transform);
	CHECK(!(other != transform));
}

TEST(TransformTest, NotEqual_Polarity) {
	Transform other;
	other.SetPolarity(Polarity::Clear);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

TEST(TransformTest, NotEqual_Mirroring) {
	Transform other;
	other.SetMirroring(Mirroring::X);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

TEST(TransformTest, NotEqual_Rotation) {
	Transform other;
	other.SetRotation(30.0);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

TEST(TransformTest, NotEqual_Scaling) {
	Transform other;
	other.SetScaling(2.25);

	CHECK(!(other == transform));
	CHECK(other != transform);
}

TEST(TransformTest, RejectsScaling) {
	CHECK_THROWS(std::invalid_argument, transform.SetScaling(0.0));
	CHECK_THROWS(std::invalid_argument, transform.SetScaling(-1.0));
}

TEST(TransformTest, PolarityFromCommand) {
	CHECK(Polarity::Clear == Transform::PolarityFromCommand("C"));
	CHECK(Polarity::Dark == Transform::PolarityFromCommand("D"));
	CHECK_THROWS(std::invalid_argument, Transform::PolarityFromCommand("Q"));
}

TEST(TransformTest, MirroringFromCommand) {
	CHECK(Mirroring::None == Transform::MirroringFromCommand("N"));
	CHECK(Mirroring::X == Transform::MirroringFromCommand("X"));
	CHECK(Mirroring::Y == Transform::MirroringFromCommand("Y"));
	CHECK(Mirroring::XY == Transform::MirroringFromCommand("XY"));
	CHECK_THROWS(std::invalid_argument, Transform::MirroringFromCommand("Q"));
}

TEST_GROUP(ApertureTransformation_Apply) {
	Transform transform;
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
	transform.SetScaling(0.6);
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
	transform.SetRotation(45.0);
	Point expected(0.0, sqrt(2.0));
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, RotateCW) {
	transform.SetRotation(-45.0);
	Point expected(sqrt(2.0), 0.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, MirrorXThenRotate) {
	transform.SetMirroring(Mirroring::X);
	transform.SetRotation(45.0);
	Point expected(-sqrt(2.0), 0.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST(ApertureTransformation_Apply, MirrorYThenRotate) {
	transform.SetMirroring(Mirroring::Y);
	transform.SetRotation(45.0);
	Point expected(sqrt(2.0), 0.0);
	Point result = transform.Apply(point);

	CHECK_EQUAL(expected, result);
}

TEST_GROUP(ApertureTransformation_Stack) {
	Transform transform;
	Transform other;

	void setup() {
		transform = Transform();
		other = Transform();
	}
};

TEST(ApertureTransformation_Stack, Scaling) {
	transform.SetScaling(0.8);
	other.SetScaling(0.5);
	Transform result = transform.Stack(other);

	DOUBLES_EQUAL(0.4, result.GetScaling(), DBL_TOL);
}

TEST(ApertureTransformation_Stack, Rotation) {
	transform.SetRotation(30.0);
	other.SetRotation(-10.0);
	Transform result = transform.Stack(other);

	DOUBLES_EQUAL(20.0, result.GetRotation(), DBL_TOL);
}

TEST(ApertureTransformation_Stack, PolarityClearIgnoresDark) {
	transform.SetPolarity(Polarity::Clear);
	other.SetPolarity(Polarity::Dark);
	Transform result = transform.Stack(other);

	CHECK(Polarity::Clear == result.GetPolarity());
}

TEST(ApertureTransformation_Stack, PolarityDarkToClear) {
	transform.SetPolarity(Polarity::Dark);
	other.SetPolarity(Polarity::Clear);
	Transform result = transform.Stack(other);

	CHECK(Polarity::Clear == result.GetPolarity());
}

TEST(ApertureTransformation_Stack, PolarityClearToDark) {
	transform.SetPolarity(Polarity::Clear);
	other.SetPolarity(Polarity::Clear);
	Transform result = transform.Stack(other);

	CHECK(Polarity::Dark == result.GetPolarity());
}

TEST(ApertureTransformation_Stack, MirrorNoneToX) {
	transform.SetMirroring(Mirroring::None);
	other.SetMirroring(Mirroring::X);
	Transform result = transform.Stack(other);

	CHECK(Mirroring::X == result.GetMirroring());
}

TEST(ApertureTransformation_Stack, MirrorXToNone) {
	transform.SetMirroring(Mirroring::X);
	other.SetMirroring(Mirroring::X);
	Transform result = transform.Stack(other);

	CHECK(Mirroring::None == result.GetMirroring());
}

TEST(ApertureTransformation_Stack, MirrorNoneToY) {
	transform.SetMirroring(Mirroring::None);
	other.SetMirroring(Mirroring::Y);
	Transform result = transform.Stack(other);

	CHECK(Mirroring::Y == result.GetMirroring());
}

TEST(ApertureTransformation_Stack, MirrorYToNone) {
	transform.SetMirroring(Mirroring::Y);
	other.SetMirroring(Mirroring::Y);
	Transform result = transform.Stack(other);

	CHECK(Mirroring::None == result.GetMirroring());
}
