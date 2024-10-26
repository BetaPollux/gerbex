/*
 * test_Macro.cpp
 *
 *  Created on: Mar. 30, 2024
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

#include "Macro.h"
#include "MacroCircle.h"
#include "MacroPolygon.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;


TEST_GROUP(MacroTest) {
};

TEST(MacroTest, Empty) {
	Macro macro;
	std::vector<std::shared_ptr<MacroPrimitive>> prims = macro.GetPrimitives();
	CHECK(prims.empty());
}

TEST(MacroTest, AddPrimitives) {
	std::shared_ptr<MacroCircle> circle = std::make_shared<MacroCircle>();
	std::shared_ptr<MacroPolygon> poly = std::make_shared<MacroPolygon>();
	Macro macro;
	macro.AddPrimitive(circle);
	macro.AddPrimitive(poly);
	std::vector<std::shared_ptr<MacroPrimitive>> prims = macro.GetPrimitives();
	LONGS_EQUAL(2, prims.size());
	POINTERS_EQUAL(prims.front().get(), circle.get());
	POINTERS_EQUAL(prims.back().get(), poly.get());
}

//TODO test serialize, get box, clone
