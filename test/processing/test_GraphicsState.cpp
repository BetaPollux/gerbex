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
#include "GraphicsStringFrom.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;

TEST_GROUP (GraphicsStateTest) {
	GraphicsState state;
};

TEST(GraphicsStateTest, UnitFromCommand) {
	CHECK(Unit::Millimeter == GraphicsState::UnitFromCommand("MOMM"));
	CHECK(Unit::Inch == GraphicsState::UnitFromCommand("MOIN"));
	CHECK_THROWS(std::invalid_argument, GraphicsState::UnitFromCommand("MOX"));
}

TEST(GraphicsStateTest, PlotStateFromCommand) {
	CHECK(PlotState::Linear == GraphicsState::PlotStateFromCommand("G01"));
	CHECK(PlotState::Clockwise == GraphicsState::PlotStateFromCommand("G02"));
	CHECK(
			PlotState::CounterClockwise
					== GraphicsState::PlotStateFromCommand("G03"));
	CHECK_THROWS(std::invalid_argument,
			GraphicsState::PlotStateFromCommand("G55"));
}

TEST(GraphicsStateTest, ArcModeFromCommand) {
	CHECK(ArcMode::SingleQuadrant == GraphicsState::ArcModeFromCommand("G74"));
	CHECK(ArcMode::MultiQuadrant == GraphicsState::ArcModeFromCommand("G75"));
	CHECK_THROWS(std::invalid_argument,
			GraphicsState::ArcModeFromCommand("G76"));
}

TEST(GraphicsStateTest, Format) {
	CHECK(!state.GetFormat().has_value());
}

TEST(GraphicsStateTest, Unit) {
	CHECK(!state.GetUnit().has_value());
}

TEST(GraphicsStateTest, CurrentPoint) {
	CHECK(!state.GetCurrentPoint().has_value());
}

TEST(GraphicsStateTest, CurrentAperture) {
	CHECK(nullptr == state.GetCurrentAperture());
}

TEST(GraphicsStateTest, PlotState) {
	CHECK(!state.GetPlotState().has_value());
}

TEST(GraphicsStateTest, ArcMode) {
	CHECK(!state.GetArcMode().has_value());
}

TEST(GraphicsStateTest, SetArcMode_RejectsSingleQuadrant) {
	CHECK_THROWS(std::invalid_argument,
			state.SetArcMode(ArcMode::SingleQuadrant));
}

TEST(GraphicsStateTest, Transform) {
	CHECK(Transform() == state.GetTransformation());
}

TEST_GROUP (GraphicsStateGetPoint) {
	GraphicsState state;

	void setup() {
		Point currentPoint(1.0, -5.0);
		state.SetFormat(CoordinateFormat(3, 6));
		state.SetCurrentPoint(currentPoint);
	}
};

TEST(GraphicsStateGetPoint, GraphicsStateGetPoint) {
	Point expected(2.5, 3.0);
	CoordinateData data(2500000, 3000000);
	Point newPt = state.GetPoint(data);
	CHECK_EQUAL(expected, newPt);
}

TEST(GraphicsStateGetPoint, NoNewData) {
	CoordinateData data(std::nullopt, std::nullopt);
	Point result = state.GetPoint(data);
	CHECK_EQUAL(*state.GetCurrentPoint(), result);
}

TEST(GraphicsStateGetPoint, OnlyNewX) {
	Point expected(2.5, state.GetCurrentPoint()->GetY());
	CoordinateData data(2500000, std::nullopt);
	Point result = state.GetPoint(data);
	CHECK_EQUAL(expected, result);
}

TEST(GraphicsStateGetPoint, OnlyNewY) {
	Point expected(state.GetCurrentPoint()->GetX(), 7.0);
	CoordinateData data(std::nullopt, 7000000);
	Point result = state.GetPoint(data);
	CHECK_EQUAL(expected, result);
}

TEST(GraphicsStateGetPoint, NoNewData_NoCurrent) {
	state.SetCurrentPoint(std::nullopt);
	CoordinateData data(std::nullopt, std::nullopt);
	CHECK_THROWS(std::invalid_argument, state.GetPoint(data));
}

TEST(GraphicsStateGetPoint, NoNewX_NoCurrent) {
	state.SetCurrentPoint(std::nullopt);
	CoordinateData data(std::nullopt, 3000000);
	CHECK_THROWS(std::invalid_argument, state.GetPoint(data));
}

TEST(GraphicsStateGetPoint, NoNewY_NoCurrent) {
	state.SetCurrentPoint(std::nullopt);
	CoordinateData data(2500000, std::nullopt);
	CHECK_THROWS(std::invalid_argument, state.GetPoint(data));
}

