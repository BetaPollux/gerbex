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

TEST(TransformTest, Mirroring) {
	CHECK(Mirroring::None == transform.GetMirroring());
}

TEST(TransformTest, Rotation) {
	DOUBLES_EQUAL(0.0, transform.GetRotation(), DBL_TOL);
}

TEST(TransformTest, Scaling) {
	DOUBLES_EQUAL(1.0, transform.GetScaling(), DBL_TOL);
}

TEST(TransformTest, Equal) {
	Transform other;

	CHECK(other == transform);
	CHECK(!(other != transform));
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

TEST(TransformTest, MirroringFromCommand) {
	CHECK(Mirroring::None == Transform::MirroringFromCommand("N"));
	CHECK(Mirroring::X == Transform::MirroringFromCommand("X"));
	CHECK(Mirroring::Y == Transform::MirroringFromCommand("Y"));
	CHECK(Mirroring::XY == Transform::MirroringFromCommand("XY"));
	CHECK_THROWS(std::invalid_argument, Transform::MirroringFromCommand("Q"));
}
