/*
 * test_CgalSerializer.cpp
 *
 *  Created on: Nov. 3, 2024
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
#include "CgalSerializer.h"
#include "Point.h"
#include "CppUTest/TestHarness.h"

namespace gerbex {

TEST_GROUP(CgalSerializer) {
	void setup() {
		//Empty
	}
};

TEST(CgalSerializer, MakeFile) {
	CgalSerializer serializer;
	pSerialItem root = serializer.GetTarget(Polarity::Dark);
	serializer.AddPolygon(root, { Point(-10.0, -10.0), Point(10.0, -10.0),
			Point(0.0, -5.0) });
	serializer.SaveFile("output.svg");
}


} /* namespace gerbex */
