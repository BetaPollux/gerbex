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

/**
 * Helper functions
 */

template <typename T> void MakeAndSetAperture(CommandsProcessor &processor, int id) {
	std::shared_ptr<T> ap = std::make_unique<T>();

	processor.ApertureDefinition(id, ap);
	processor.SetCurrentAperture(id);
}

template <typename T> std::shared_ptr<T> GetGraphicalObject(CommandsProcessor &processor) {
	CHECK(processor.GetObjects().size() > 0);

	std::shared_ptr<GraphicalObject> obj = processor.GetObjects().back();
	std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(obj);

	CHECK(result != nullptr);

	return result;
}

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
	LONGS_EQUAL(PlotState::Undefined, processor.GetGraphicsState().GetPlotState());
}

TEST(CommandsProcessor_Init, Flash_NeedsCurrentAperture) {
	Point origin(250, 1200);

	CHECK_THROWS(std::logic_error, processor.Flash(origin));
}

TEST(CommandsProcessor_Init, Draw_NeedsCurrentAperture) {
	Point origin(500, 2500);
	Point end(750, -500);

	processor.SetPlotState(PlotState::Linear);
	processor.Move(origin);

	CHECK_THROWS(std::logic_error, processor.PlotDraw(end));
}

TEST(CommandsProcessor_Init, Arc_NeedsCurrentAperture) {
	Point origin(3000, -2000);
	Point end(-3000, 4000);
	Point offset(-3000, -2000);

	processor.SetPlotState(PlotState::Clockwise);
	processor.Move(origin);

	CHECK_THROWS(std::logic_error, processor.PlotArc(end, offset));
}

TEST(CommandsProcessor_Init, Move) {
	Point pt(1000, 3000);

	MakeAndSetAperture<Circle>(processor, 10);
	processor.Move(pt);

	LONGS_EQUAL(0, processor.GetObjects().size());
	CHECK(pt == *processor.GetGraphicsState().GetCurrentPoint());
}

TEST(CommandsProcessor_Init, Draw_RequiresCurrentPoint) {
	Point end(-250, 0);

	MakeAndSetAperture<Circle>(processor, 10);
	processor.SetPlotState(PlotState::Linear);
	//Current point is invalid on initialization

	CHECK_THROWS(std::logic_error, processor.PlotDraw(end));
}

TEST(CommandsProcessor_Init, Arc_RequiresCurrentPoint) {
	Point end(-3000, 4000);
	Point offset(-3000, -2000);

	MakeAndSetAperture<Circle>(processor, 10);
	processor.SetPlotState(PlotState::Clockwise);
	//Current point is invalid on initialization

	CHECK_THROWS(std::logic_error, processor.PlotArc(end, offset));
}

/***
 * Tests for Flash operation
 */

TEST_GROUP(CommandsProcessor_Flash) {
	Point origin;
	CommandsProcessor processor;

	void setup() {
		origin = Point(3000, -2000);

		MakeAndSetAperture<Circle>(processor, 10);
	}
};

TEST(CommandsProcessor_Flash, MakesOne) {
	processor.Flash(origin);

	LONGS_EQUAL(1, processor.GetObjects().size());
}

TEST(CommandsProcessor_Flash, Origin) {
	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(processor);

	CHECK(origin == flash->GetOrigin());
}

TEST(CommandsProcessor_Flash, Aperture) {
	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(processor);

	POINTERS_EQUAL(processor.GetGraphicsState().GetCurrentAperture().get(),
			flash->GetAperture().get());
}

TEST(CommandsProcessor_Flash, Transform) {
	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(processor);

	CHECK(processor.GetGraphicsState().GetTransformation() ==
				flash->GetTransformation());
}

TEST(CommandsProcessor_Flash, SetsCurrentPoint) {
	processor.Flash(origin);

	CHECK(origin == *processor.GetGraphicsState().GetCurrentPoint());
}

/***
 * Tests for Plot Draw operation
 */

TEST_GROUP(CommandsProcessor_PlotDraw) {
	Point origin, end;
	CommandsProcessor processor;

	void setup() {
		origin = Point(3000, -2000);
		end = Point(750, -500);

		MakeAndSetAperture<Circle>(processor, 10);
		processor.SetPlotState(PlotState::Linear);
		processor.Move(origin);
	}
};

TEST(CommandsProcessor_PlotDraw, BadState) {
	std::array<PlotState, 3> state = {
			PlotState::Undefined,
			//Omit Linear
			PlotState::Clockwise,
			PlotState::CounterClockwise
	};

	for(PlotState st : state) {
		processor.SetPlotState(st);
		CHECK_THROWS(std::logic_error, processor.PlotDraw(end));
	}
}

TEST(CommandsProcessor_PlotDraw, MakesOne) {
	processor.PlotDraw(end);

	LONGS_EQUAL(1, processor.GetObjects().size());
}

TEST(CommandsProcessor_PlotDraw, Origin) {
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(processor);

	CHECK(origin == draw->GetOrigin());
}

TEST(CommandsProcessor_PlotDraw, End) {
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(processor);

	CHECK(end == draw->GetEndPoint());
}

TEST(CommandsProcessor_PlotDraw, Aperture) {
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(processor);

	POINTERS_EQUAL(processor.GetGraphicsState().GetCurrentAperture().get(),
			draw->GetAperture().get());
}

TEST(CommandsProcessor_PlotDraw, Transform) {
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(processor);

	CHECK(processor.GetGraphicsState().GetTransformation() ==
				draw->GetTransformation());
}

TEST(CommandsProcessor_PlotDraw, SetsCurrentPoint) {
	processor.PlotDraw(end);

	CHECK(end == *processor.GetGraphicsState().GetCurrentPoint());
}

/***
 * Tests for Plot Arc operation
 */

TEST_GROUP(CommandsProcessor_PlotArc) {
	Point origin, end, offset;
	CommandsProcessor processor;

	void setup() {
		origin = Point(3000, -2000);
		end = Point(-3000, 4000);
		offset = Point(-3000, -2000);

		MakeAndSetAperture<Circle>(processor, 10);
		processor.SetPlotState(PlotState::Clockwise);
		processor.Move(origin);
	}
};

TEST(CommandsProcessor_PlotArc, BadState) {
	std::array<PlotState, 2> state = {
			PlotState::Undefined,
			PlotState::Linear,
			//Omit Clockwise and CounterClockwise
	};

	for(PlotState st : state) {
		processor.SetPlotState(st);
		CHECK_THROWS(std::logic_error, processor.PlotArc(end, offset));
	}
}

TEST(CommandsProcessor_PlotArc, MakesOne) {
	processor.PlotArc(end, offset);

	LONGS_EQUAL(1, processor.GetObjects().size());
}

TEST(CommandsProcessor_PlotArc, SetsCurrentPoint) {
	processor.PlotArc(end, offset);

	CHECK(end == *processor.GetGraphicsState().GetCurrentPoint());
}

TEST(CommandsProcessor_PlotArc, Origin) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor);

	CHECK(origin == arc->GetOrigin());
}

TEST(CommandsProcessor_PlotArc, End) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor);

	CHECK(end == arc->GetEndPoint());
}

TEST(CommandsProcessor_PlotArc, Offset) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor);

	CHECK(offset == arc->GetCenterOffset());
}

TEST(CommandsProcessor_PlotArc, Direction_CW) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor);

	CHECK(ArcDirection::Clockwise == arc->GetDirection());
}

TEST(CommandsProcessor_PlotArc, Direction_CCW) {
	processor.SetPlotState(PlotState::CounterClockwise);
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor);

	CHECK(ArcDirection::CounterClockwise == arc->GetDirection());
}

TEST(CommandsProcessor_PlotArc, Aperture) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor);

	POINTERS_EQUAL(processor.GetGraphicsState().GetCurrentAperture().get(),
			arc->GetAperture().get());
}

TEST(CommandsProcessor_PlotArc, Transform) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor);

	CHECK(processor.GetGraphicsState().GetTransformation() ==
				arc->GetTransformation());
}

