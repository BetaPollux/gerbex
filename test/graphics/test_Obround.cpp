/*
 * test_Obround.cpp
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

#include "Obround.h"
#include "GraphicsStringFrom.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

#define DBL_TOL	1e-9

using namespace gerbex;


TEST_GROUP(ObroundTest) {
};

TEST(ObroundTest, ZeroXSize) {
	CHECK_THROWS(std::invalid_argument, Obround(0.0, 1.0));
}

TEST(ObroundTest, ZeroYSize) {
	CHECK_THROWS(std::invalid_argument, Obround(1.0, 0.0));
}

TEST(ObroundTest, NegativeHoleSize) {
	CHECK_THROWS(std::invalid_argument, Obround(1.0, 1.0, -0.25));
}

TEST(ObroundTest, Length) {
	Obround obround(1.0, 0.5);
	DOUBLES_EQUAL(1.0, obround.GetLength(), DBL_TOL);
}

TEST(ObroundTest, DrawWidth) {
	Obround obround(1.0, 0.5);
	DOUBLES_EQUAL(0.5, obround.GetDrawWidth(), DBL_TOL);
}

TEST(ObroundTest, DefaultHole) {
	Obround obround(1.0, 0.5);
	DOUBLES_EQUAL(0.0, obround.GetHoleDiameter(), DBL_TOL);
}

TEST(ObroundTest, Box) {
	double w = 2.0;
	double h = 1.0;
	Obround obround(w, h);

	Box expected(w, h, -0.5 * w, -0.5 * h);
	CHECK_EQUAL(expected, obround.GetBox());
}

TEST_GROUP(Obround_Transformed) {
	Transform transform;
	Obround obround;

	void setup() {
		transform = Transform();
		transform.SetScaling(2.0);
		transform.SetRotation(90.0);
		transform.SetPolarity(Polarity::Clear);

		obround = Obround(1.0, 0.5, 0.25);
		obround.ApplyTransform(transform);
	}
};

TEST(Obround_Transformed, Length) {
	DOUBLES_EQUAL(2.0, obround.GetLength(), DBL_TOL);
}

TEST(Obround_Transformed, DrawWidth) {
	DOUBLES_EQUAL(1.0, obround.GetDrawWidth(), DBL_TOL);
}

TEST(Obround_Transformed, HoleDiameter) {
	DOUBLES_EQUAL(0.5, obround.GetHoleDiameter(), DBL_TOL);
}

TEST(Obround_Transformed, Polarity) {
	CHECK_EQUAL((int)Polarity::Clear, (int)obround.GetPolarity());
}

TEST(Obround_Transformed, Box) {
	Box expected(1.0, 2.0, -0.5, -1.0);
	CHECK_EQUAL(expected, obround.GetBox());
}

TEST(Obround_Transformed, Clone) {
	std::unique_ptr<Aperture> aperture = obround.Clone();
	Obround *clone = (Obround*) aperture.get();

	CHECK(clone != &obround);
	CHECK_EQUAL(obround, *clone);
}

// TODO test obround serialize
