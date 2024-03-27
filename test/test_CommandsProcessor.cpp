/*
 * test_CommandsProcessor.cpp
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

#include "Arc.h"
#include "CommandsProcessor.h"
#include "Circle.h"
#include "Draw.h"
#include "Flash.h"
#include <array>
#include <stdexcept>
#include "CppUTest/TestHarness.h"

/***
 * Tests from a clean initialization.
 */

TEST_GROUP(CommandsProcessor_Init) {
	CommandsProcessor processor;
};

TEST(CommandsProcessor_Init, State) {
	LONGS_EQUAL(CommandState::Normal, processor.GetCommandState());
}

TEST(CommandsProcessor_Init, StandardTemplates) {
	CHECK(processor.GetTemplate("C") != nullptr);
	CHECK(processor.GetTemplate("R") != nullptr);
	CHECK(processor.GetTemplate("O") != nullptr);
	CHECK(processor.GetTemplate("P") != nullptr);
}

TEST(CommandsProcessor_Init, GetTemplate_DoesNotExist) {
	CHECK_THROWS(std::invalid_argument, processor.GetTemplate("X"));
}

TEST(CommandsProcessor_Init, SetCurrentAperture_DoesNotExist) {
	CHECK_THROWS(std::invalid_argument, processor.SetCurrentAperture(10));
}

TEST(CommandsProcessor_Init, ApertureDefinition) {
	std::shared_ptr<Circle> circle = std::make_unique<Circle>();

	processor.ApertureDefinition(10, circle);
	processor.SetCurrentAperture(10);

	POINTERS_EQUAL(circle.get(), processor.GetGraphicsState().GetCurrentAperture().get());
}

TEST(CommandsProcessor_Init, ApertureDefinition_BadNumber) {
	//0 through 9 are illegal
	std::unique_ptr<Circle> circle = std::make_unique<Circle>();

	CHECK_THROWS(std::invalid_argument, processor.ApertureDefinition(9, std::move(circle)));
}

TEST(CommandsProcessor_Init, ApertureDefinition_Null) {
	CHECK_THROWS(std::invalid_argument, processor.ApertureDefinition(10, nullptr));
}

TEST(CommandsProcessor_Init, PlotState) {
	LONGS_EQUAL(PlotState::Invalid, processor.GetGraphicsState().GetPlotState());
}

TEST(CommandsProcessor_Init, PlotDraw_BadState) {
	Point pt;
	std::array<PlotState, 3> state = {
			PlotState::Invalid,
			//Omit Linear
			PlotState::Clockwise,
			PlotState::CounterClockwise
	};

	for(PlotState st : state) {
		processor.SetPlotState(st);
		CHECK_THROWS(std::logic_error, processor.PlotDraw(pt));
	}
}

TEST(CommandsProcessor_Init, PlotArc_BadState) {
	Point pt, pt2;
	std::array<PlotState, 2> state = {
			PlotState::Invalid,
			PlotState::Linear,
			//Omit Clockwise and CounterClockwise
	};

	for(PlotState st : state) {
		processor.SetPlotState(st);
		CHECK_THROWS(std::logic_error, processor.PlotArc(pt, pt2));
	}
}

/***
 * Tests for operations post initialization
 */

TEST_GROUP(CommandsProcessor) {
	CommandsProcessor processor;

	void setup() {
		std::shared_ptr<Circle> circle = std::make_unique<Circle>();

		processor.ApertureDefinition(10, circle);
		processor.SetCurrentAperture(10);
	}
};

TEST(CommandsProcessor, Move) {
	Point pt(1000, 3000);
	processor.Move(pt);

	LONGS_EQUAL(0, processor.GetObjects().size());
	CHECK(pt == processor.GetGraphicsState().GetCurrentPoint());
}

TEST(CommandsProcessor, Flash) {
	Point pt(500, 2500);
	processor.Flash(pt);

	LONGS_EQUAL(1, processor.GetObjects().size());

	std::shared_ptr<GraphicalObject> obj = processor.GetObjects().back();
	std::shared_ptr<Flash> flash = std::dynamic_pointer_cast<Flash>(obj);

	CHECK(flash != nullptr);
	CHECK(pt == flash->GetOrigin());
	POINTERS_EQUAL(processor.GetGraphicsState().GetCurrentAperture().get(),
			flash->GetAperture().get());
	CHECK(processor.GetGraphicsState().GetTransformation() ==
				flash->GetTransformation());
}

TEST(CommandsProcessor, Flash_SetsCurrentPoint) {
	Point pt(250, 1200);
	processor.Flash(pt);

	CHECK(pt == processor.GetGraphicsState().GetCurrentPoint());
}

