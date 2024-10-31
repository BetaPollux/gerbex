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

TEST(SvgSerializerTest, Empty) {
	Box box;
	SvgSerializer serializer(box);
	serializer.SaveFile("empty.svg");
}

TEST(SvgSerializerTest, MakeFile) {
	SvgSerializer serializer(Box(20.0, 20.0, -10.0, -10.0));
	serializer.SetViewPort(400, 400);
	pSerialItem root = serializer.GetTarget(Polarity::Dark());
	serializer.AddCircle(root, 0.1, Point(-10.0, -10.0));
	serializer.AddCircle(root, 0.1, Point(10.0, -10.0));
	serializer.AddCircle(root, 0.1, Point(10.0, 10.0));
	serializer.AddCircle(root, 0.1, Point(-10.0, 10.0));
	serializer.AddCircle(root, 5.0, Point(0.0, 0.0));
	serializer.AddPolygon(root, { Point(-10.0, -10.0), Point(10.0, -10.0),
			Point(0.0, -5.0) });
	serializer.SaveFile("output.svg");
}

TEST(SvgSerializerTest, Donut) {
	Box area(1000.0, 1000.0, 0.0, 0.0);
	Point center(500, 500);
	SvgSerializer serializer(area);
	serializer.SetForeground("yellow");
	serializer.SetBackground("blue");
	pSerialItem root = serializer.GetTarget(Polarity::Dark());
	pSerialItem circle = serializer.AddCircle(root, 100, center);
	pSerialItem hole = serializer.NewMask(area);
	serializer.AddCircle(hole, 40, center);
	serializer.SetMask(circle, hole);
	serializer.SaveFile("donut.svg");
}

} /* namespace gerbex */
