/*
 * test_GraphicsState.cpp
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

#include <stdexcept>

#include "GraphicsState.h"
#include "CppUTest/TestHarness.h"


TEST_GROUP (GraphicsStateTest) {
	GraphicsState state;
};

TEST(GraphicsStateTest, UnitFromCommand)
{
	CHECK(Unit::Millimeter == GraphicsState::UnitFromCommand("MOMM"));
	CHECK(Unit::Inch == GraphicsState::UnitFromCommand("MOIN"));
	CHECK_THROWS(std::invalid_argument, GraphicsState::UnitFromCommand("MOX"));
}

TEST(GraphicsStateTest, Format)
{
	CHECK(!state.GetFormat().has_value());
}

TEST(GraphicsStateTest, Unit)
{
	CHECK(Unit::Undefined == state.GetUnit());
}

TEST(GraphicsStateTest, CurrentPoint)
{
	CHECK(!state.GetCurrentPoint().has_value());
}

TEST(GraphicsStateTest, CurrentAperture)
{
	CHECK(nullptr == state.GetCurrentAperture());
}

TEST(GraphicsStateTest, PlotState)
{
	CHECK(PlotState::Undefined == state.GetPlotState());
}

TEST(GraphicsStateTest, Transform)
{
	CHECK(ApertureTransformation() == state.GetTransformation());
}


