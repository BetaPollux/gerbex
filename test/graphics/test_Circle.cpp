/*
 * test_Circle.cpp
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
#include "GraphicsStringFrom.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

#define DBL_TOL	1e-9

using namespace gerbex;

TEST_GROUP(CircleTest) {
};

TEST(CircleTest, NegativeDiameter) {
	CHECK_THROWS(std::invalid_argument, Circle(-0.5));
}

TEST(CircleTest, NegativeHoleDiameter) {
	CHECK_THROWS(std::invalid_argument, Circle(1.25, -0.5));
}

TEST(CircleTest, Diameter) {
	Circle circle(2.3);
	DOUBLES_EQUAL(2.3, circle.GetDiameter(), DBL_TOL);
}

TEST(CircleTest, ZeroDiameter) {
	Circle circle(0.0);
	DOUBLES_EQUAL(0.0, circle.GetDiameter(), DBL_TOL);
}

TEST(CircleTest, DefaultHole) {
	Circle circle(1.5);
	DOUBLES_EQUAL(0.0, circle.GetHoleDiameter(), DBL_TOL);
}

TEST(CircleTest, Polarity) {
	Circle circle;
	CHECK_EQUAL((int)Polarity::Dark, (int)circle.GetPolarity());
}

TEST(CircleTest, Box) {
	double d = 10.0;
	Box expected(d, Point());

	Circle circle(d);

	CHECK_EQUAL(expected, circle.GetBox());
}

TEST_GROUP(CircleEquality) {
	Circle base = Circle(1.0, 0.0);
	Circle diameter = Circle(2.5, 0.0);
	Circle hole = Circle(1.0, 0.5);
	Circle polarity = Circle(1.0, 0.0);

	void setup() {
		polarity.SetPolarity(Polarity::Clear);
	}
};

TEST(CircleEquality, Same) {
	CHECK(base == base);
	CHECK(!(base != base));
}

TEST(CircleEquality, Diameter) {
	CHECK(base != diameter);
	CHECK(!(base == diameter));
}

TEST(CircleEquality, Hole) {
	CHECK(base != hole);
	CHECK(!(base == hole));
}

TEST(CircleEquality, Polarity) {
	CHECK(base != polarity);
	CHECK(!(base == polarity));
}

TEST_GROUP(Circle_Transformed) {
	Transform transform;
	Circle circle;

	void setup() {
		transform = Transform();
		transform.SetScaling(3.0);
		transform.SetPolarity(Polarity::Clear);

		circle = Circle(1.5, 0.5);
		circle.ApplyTransform(transform);
	}
};

TEST(Circle_Transformed, Diameter) {
	DOUBLES_EQUAL(4.5, circle.GetDiameter(), DBL_TOL);
}

TEST(Circle_Transformed, HoleDiameter) {
	DOUBLES_EQUAL(1.5, circle.GetHoleDiameter(), DBL_TOL);
}

TEST(Circle_Transformed, Polarity) {
	CHECK_EQUAL((int)Polarity::Clear, (int)circle.GetPolarity());
}

TEST(Circle_Transformed, Box) {
	Box expected(4.5, Point());
	CHECK_EQUAL(expected, circle.GetBox());
}

TEST(Circle_Transformed, Clone) {
	std::unique_ptr<Aperture> aperture = circle.Clone();
	Circle *clone = (Circle*)aperture.get();

	CHECK(clone != &circle);
	CHECK_EQUAL(circle, *clone);
}

// TODO test circle serialize
