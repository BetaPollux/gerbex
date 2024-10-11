/*
 * test_SvgSerializer.cpp
 *
 *  Created on: Oct. 9, 2024
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

#include "SvgSerializer.h"
#include "CppUTest/TestHarness.h"

namespace gerbex {

TEST_GROUP(SvgSerializerTest) {
};

TEST(SvgSerializerTest, MakeFile) {
	SvgSerializer serializer;
	serializer.SetViewPort(800, 800);
	serializer.SetViewBox(-10.0, -10.0, 20.0, 20.0);
	serializer.AddCircle(0.1, -10.0, -10.0);
	serializer.AddCircle(0.1, 10.0, -10.0);
	serializer.AddCircle(0.1, 10.0, 10.0);
	serializer.AddCircle(0.1, -10.0, 10.0);
	serializer.AddCircle(5.0, 0.0, 0.0);
	serializer.AddRectangle(10.0, 5.0, -5.0, 5.0);
	serializer.AddPolygon(
			{ { -10.0, -10.0 }, { 10.0, -10.0 }, { 0.0, -5.0 } });
	serializer.SaveFile("output.svg");
}

} /* namespace gerbex */
