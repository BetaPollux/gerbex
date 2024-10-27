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
#include "BlockAperture.h"
#include "CastHelpers.h"
#include "CommandsProcessor.h"
#include "Circle.h"
#include "CircleTemplate.h"
#include "Draw.h"
#include "Flash.h"
#include "Region.h"
#include "StepAndRepeat.h"
#include "GraphicsStringFrom.h"
#include <array>
#include <stdexcept>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

/***
 * Tests from a clean initialization.
 */

TEST_GROUP(CommandsProcessor_Init) {
	CommandsProcessor processor;
};

TEST(CommandsProcessor_Init, State) {
	LONGS_EQUAL(CommandState::Normal, processor.GetCommandState());
}

TEST(CommandsProcessor_Init, SetEndOfFile) {
	processor.SetEndOfFile();
	LONGS_EQUAL(CommandState::EndOfFile, processor.GetCommandState());
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

TEST(CommandsProcessor_Init, AddTemplate_AlreadyExists) {
	std::shared_ptr<CircleTemplate> tmpl = std::make_shared<CircleTemplate>();
	CHECK_THROWS(std::invalid_argument, processor.AddTemplate("C", tmpl));
}

TEST(CommandsProcessor_Init, AddTemplate) {
	std::shared_ptr<CircleTemplate> tmpl = std::make_shared<CircleTemplate>();
	processor.AddTemplate("Triangle_30", tmpl);
	CHECK(processor.GetTemplate("Triangle_30") == tmpl);
}

TEST(CommandsProcessor_Init, SetCurrentAperture_DoesNotExist) {
	CHECK_THROWS(std::invalid_argument, processor.SetCurrentAperture(10));
}

TEST(CommandsProcessor_Init, ApertureDefine) {
	std::shared_ptr<Circle> circle = std::make_unique<Circle>();

	processor.ApertureDefine(10, circle);
	processor.SetCurrentAperture(10);

	POINTERS_EQUAL(circle.get(),
			processor.GetGraphicsState().GetCurrentAperture().get());
}

TEST(CommandsProcessor_Init, ApertureDefine_BadNumber) {
	//0 through 9 are illegal
	std::unique_ptr<Circle> circle = std::make_unique<Circle>();

	CHECK_THROWS(std::invalid_argument,
			processor.ApertureDefine(9, std::move(circle)));
}

TEST(CommandsProcessor_Init, ApertureDefine_CannotReassign) {
	int ident = 11;
	std::unique_ptr<Circle> circle = std::make_unique<Circle>();
	std::unique_ptr<Circle> other_circle = std::make_unique<Circle>();

	processor.ApertureDefine(ident, std::move(circle));
	CHECK_THROWS(std::invalid_argument,
			processor.ApertureDefine(ident, std::move(other_circle)));
}

TEST(CommandsProcessor_Init, ApertureDefine_Null) {
	CHECK_THROWS(std::invalid_argument, processor.ApertureDefine(10, nullptr));
}

TEST(CommandsProcessor_Init, Flash_NeedsCurrentAperture) {
	Point origin(250, 1200);

	CHECK_THROWS(std::logic_error, processor.Flash(origin));
}

TEST(CommandsProcessor_Init, Draw_NeedsCurrentAperture) {
	Point origin(500, 2500);
	Point end(750, -500);

	processor.GetGraphicsState().SetPlotState(PlotState::Linear);
	processor.Move(origin);

	CHECK_THROWS(std::logic_error, processor.PlotDraw(end));
}

TEST(CommandsProcessor_Init, Arc_NeedsCurrentAperture) {
	Point origin(3000, -2000);
	Point end(-3000, 4000);
	Point offset(-3000, -2000);

	processor.GetGraphicsState().SetPlotState(PlotState::Clockwise);
	processor.Move(origin);

	CHECK_THROWS(std::logic_error, processor.PlotArc(end, offset));
}

TEST(CommandsProcessor_Init, Move) {
	Point pt(1000, 3000);

	MakeAndSetAperture<Circle>(processor, 10);
	processor.Move(pt);

	LONGS_EQUAL(0, processor.GetObjects().size());
	CHECK_EQUAL(pt, *processor.GetGraphicsState().GetCurrentPoint());
}

TEST(CommandsProcessor_Init, Draw_RequiresCurrentPoint) {
	Point end(-250, 0);

	MakeAndSetAperture<Circle>(processor, 10);
	processor.GetGraphicsState().SetPlotState(PlotState::Linear);
	//Current point is invalid on initialization

	CHECK_THROWS(std::logic_error, processor.PlotDraw(end));
}

TEST(CommandsProcessor_Init, Arc_RequiresCurrentPoint) {
	Point end(-3000, 4000);
	Point offset(-3000, -2000);

	MakeAndSetAperture<Circle>(processor, 10);
	processor.GetGraphicsState().SetPlotState(PlotState::Clockwise);
	//Current point is invalid on initialization

	CHECK_THROWS(std::logic_error, processor.PlotArc(end, offset));
}

/***
 * Tests for Flash operation
 */

TEST_GROUP(CommandsProcessor_Flash) {
	Point origin;
	CommandsProcessor processor;
	std::shared_ptr<Circle> circle;

	void setup() {
		origin = Point(3.0, -2.0);

		int id = 12;
		circle = std::make_shared<Circle>(5.0, 0.75);
		processor.ApertureDefine(id, circle);
		processor.SetCurrentAperture(id);
	}
};

TEST(CommandsProcessor_Flash, MakesOne) {
	processor.Flash(origin);

	LONGS_EQUAL(1, processor.GetObjects().size());
}

TEST(CommandsProcessor_Flash, Origin) {
	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(
			processor.GetObjects());

	CHECK_EQUAL(origin, flash->GetOrigin());
}

TEST(CommandsProcessor_Flash, Aperture) {
	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(
			processor.GetObjects());

	CHECK_EQUAL(*circle, *(Circle* )(flash->GetAperture().get()));
}

TEST(CommandsProcessor_Flash, Transform) {
	Transform transform(Polarity::Clear, Mirroring::X, 30.0, 1.5);
	processor.GetGraphicsState().SetTransform(transform);
	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(
			processor.GetObjects());

	CHECK_EQUAL(transform, flash->GetAperture()->GetTransform());
}

TEST(CommandsProcessor_Flash, SetsCurrentPoint) {
	processor.Flash(origin);

	CHECK_EQUAL(origin, *processor.GetGraphicsState().GetCurrentPoint());
}

/***
 * Tests for Plot Draw operation
 */

TEST_GROUP(CommandsProcessor_PlotDraw) {
	Point origin, end;
	CommandsProcessor processor;
	std::shared_ptr<Circle> circle;

	void setup() {
		origin = Point(3.0, -2.0);
		end = Point(0.75, -0.5);

		int id = 12;
		circle = std::make_shared<Circle>(5.0, 0.75);
		processor.ApertureDefine(id, circle);
		processor.SetCurrentAperture(id);
		processor.GetGraphicsState().SetPlotState(PlotState::Linear);
		processor.Move(origin);
	}
};

TEST(CommandsProcessor_PlotDraw, BadState) {
	std::array<PlotState, 2> state = {
//Omit Linear
			PlotState::Clockwise, PlotState::CounterClockwise };

	for (PlotState st : state) {
		processor.GetGraphicsState().SetPlotState(st);
		CHECK_THROWS(std::logic_error, processor.PlotDraw(end));
	}
}

TEST(CommandsProcessor_PlotDraw, MakesOne) {
	processor.PlotDraw(end);

	LONGS_EQUAL(1, processor.GetObjects().size());
}

TEST(CommandsProcessor_PlotDraw, Origin) {
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(
			processor.GetObjects());

	CHECK_EQUAL(origin, draw->GetSegment().GetStart());
}

TEST(CommandsProcessor_PlotDraw, End) {
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(
			processor.GetObjects());

	CHECK_EQUAL(end, draw->GetSegment().GetEnd());
}

TEST(CommandsProcessor_PlotDraw, Aperture) {
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(
			processor.GetObjects());

	CHECK_EQUAL(*circle, *(Circle* )(draw->GetAperture().get()));
}

TEST(CommandsProcessor_PlotDraw, Transform) {
	Transform transform(Polarity::Clear, Mirroring::Y, 15.0, 0.5);
	processor.GetGraphicsState().SetTransform(transform);
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(
			processor.GetObjects());

	CHECK_EQUAL(transform, draw->GetAperture()->GetTransform());
}

TEST(CommandsProcessor_PlotDraw, SetsCurrentPoint) {
	processor.PlotDraw(end);

	CHECK_EQUAL(end, *processor.GetGraphicsState().GetCurrentPoint());
}

/***
 * Tests for Plot Arc operation
 */

TEST_GROUP(CommandsProcessor_PlotArc) {
	Point origin, end, offset;
	CommandsProcessor processor;
	std::shared_ptr<Circle> circle;

	void setup() {
		origin = Point(3.0, -2.0);
		end = Point(-3.0, 4.0);
		offset = Point(-3.0, -2.0);

		int id = 12;
		circle = std::make_shared<Circle>(5.0, 0.75);
		processor.ApertureDefine(id, circle);
		processor.SetCurrentAperture(id);
		processor.GetGraphicsState().SetPlotState(PlotState::Clockwise);
		processor.Move(origin);
	}
};

TEST(CommandsProcessor_PlotArc, BadState) {
	std::array<PlotState, 1> state = { PlotState::Linear,
//Omit Clockwise and CounterClockwise
			};

	for (PlotState st : state) {
		processor.GetGraphicsState().SetPlotState(st);
		CHECK_THROWS(std::logic_error, processor.PlotArc(end, offset));
	}
}

TEST(CommandsProcessor_PlotArc, MakesOne) {
	processor.PlotArc(end, offset);

	LONGS_EQUAL(1, processor.GetObjects().size());
}

TEST(CommandsProcessor_PlotArc, SetsCurrentPoint) {
	processor.PlotArc(end, offset);

	CHECK_EQUAL(end, *processor.GetGraphicsState().GetCurrentPoint());
}

TEST(CommandsProcessor_PlotArc, Origin) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor.GetObjects());

	CHECK_EQUAL(origin, arc->GetSegment().GetStart());
}

TEST(CommandsProcessor_PlotArc, End) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor.GetObjects());

	CHECK_EQUAL(end, arc->GetSegment().GetEnd());
}

TEST(CommandsProcessor_PlotArc, Offset) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor.GetObjects());

	CHECK_EQUAL(offset, arc->GetSegment().GetCenterOffset());
}

TEST(CommandsProcessor_PlotArc, Direction_CW) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor.GetObjects());

	CHECK(ArcDirection::Clockwise == arc->GetSegment().GetDirection());
}

TEST(CommandsProcessor_PlotArc, Direction_CCW) {
	processor.GetGraphicsState().SetPlotState(PlotState::CounterClockwise);
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor.GetObjects());

	CHECK(ArcDirection::CounterClockwise == arc->GetSegment().GetDirection());
}

TEST(CommandsProcessor_PlotArc, Aperture) {
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor.GetObjects());

	CHECK_EQUAL(*circle, *(Circle* )(arc->GetAperture().get()));
}

TEST(CommandsProcessor_PlotArc, Transform) {
	Transform transform(Polarity::Dark, Mirroring::Y, 10.0, 1.25);
	processor.GetGraphicsState().SetTransform(transform);
	processor.PlotArc(end, offset);

	std::shared_ptr<Arc> arc = GetGraphicalObject<Arc>(processor.GetObjects());

	CHECK_EQUAL(transform, arc->GetAperture()->GetTransform());
}

/***
 * Tests for Region operation -- inside region
 */

TEST_GROUP(CommandsProcessor_InsideRegion) {
	Point origin, end, offset;
	CommandsProcessor processor;

	void setup() {
		origin = Point(3000, -2000);
		end = Point(750, -500);
		offset = Point(-2000, 1000);

		processor.GetGraphicsState().SetPlotState(PlotState::Linear);
		processor.StartRegion();
	}
};

TEST(CommandsProcessor_InsideRegion, SetsState) {
	CHECK(CommandState::InsideRegion == processor.GetCommandState());
}

TEST(CommandsProcessor_InsideRegion, CannotStartRegion) {
	CHECK_THROWS(std::logic_error, processor.StartRegion());
}

TEST(CommandsProcessor_InsideRegion, CreatesRegion_NoContour) {
	processor.EndRegion();

	std::shared_ptr<Region> region = GetGraphicalObject<Region>(
			processor.GetObjects());
	LONGS_EQUAL(0, region->GetContours().size());
}

TEST(CommandsProcessor_InsideRegion, StartsContour) {
	processor.Move(origin);
	processor.EndRegion();

	std::shared_ptr<Region> region = GetGraphicalObject<Region>(
			processor.GetObjects());
	LONGS_EQUAL(1, region->GetContours().size());
}

TEST(CommandsProcessor_InsideRegion, AddsSegment_Draw) {
	processor.Move(origin);
	processor.PlotDraw(end);
	processor.EndRegion();

	std::shared_ptr<Region> region = GetGraphicalObject<Region>(
			processor.GetObjects());
	LONGS_EQUAL(1, region->GetContours().size());
	const Contour &contour = region->GetContours().back();

	LONGS_EQUAL(1, contour.GetSegments().size());
}

TEST(CommandsProcessor_InsideRegion, AddsSegment_Arc) {
	processor.Move(origin);
	processor.GetGraphicsState().SetPlotState(PlotState::Clockwise);
	processor.PlotArc(end, offset);
	processor.EndRegion();

	std::shared_ptr<Region> region = GetGraphicalObject<Region>(
			processor.GetObjects());
	LONGS_EQUAL(1, region->GetContours().size());
	const Contour &contour = region->GetContours().back();

	LONGS_EQUAL(1, contour.GetSegments().size());
}

/***
 * Tests for Region operation -- after region
 */

TEST_GROUP(CommandsProcessor_AfterRegion) {
	Point origin, mid, end;
	CommandsProcessor processor;

	void setup() {
		origin = Point(3000, -2000);
		mid = Point(1500, 1000);
		end = Point(750, -500);

		Transform transformation;
		transformation.SetPolarity(Polarity::Clear);
		processor.GetGraphicsState().SetPlotState(PlotState::Linear);
		processor.GetGraphicsState().SetTransform(transformation);
		processor.StartRegion();
		processor.Move(origin);
		processor.PlotDraw(mid);
		processor.PlotDraw(end);
		processor.PlotDraw(origin);
		processor.EndRegion();
	}
};

TEST(CommandsProcessor_AfterRegion, AddsDrawsAgain) {
	MakeAndSetAperture<Circle>(processor, 10);
	processor.Move(origin);
	processor.PlotDraw(end);

	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(
			processor.GetObjects(), 1);
	CHECK_EQUAL(end, draw->GetSegment().GetEnd());
}

TEST(CommandsProcessor_AfterRegion, ClearsState) {
	CHECK(CommandState::Normal == processor.GetCommandState());
}

TEST(CommandsProcessor_AfterRegion, CreatesRegion) {
	std::shared_ptr<Region> region = GetGraphicalObject<Region>(
			processor.GetObjects());
}

TEST(CommandsProcessor_AfterRegion, CannotEndRegion) {
	CHECK_THROWS(std::logic_error, processor.EndRegion());
}

TEST(CommandsProcessor_AfterRegion, TakesPolarity) {
	std::shared_ptr<Region> region = GetGraphicalObject<Region>(
			processor.GetObjects());

	CHECK(Polarity::Clear == region->GetPolarity());
}

/***
 * Tests for Aperture Block
 */

TEST_GROUP(CommandsProcessor_ApertureBlock) {
	Point origin, end;
	CommandsProcessor processor;
	int blockId, circleId;

	void setup() {
		origin = Point(3000, -2000);
		end = Point(750, -500);
		circleId = 10;
		blockId = 12;

		processor.GetGraphicsState().SetPlotState(PlotState::Linear);
		MakeAndSetAperture<Circle>(processor, circleId);
		processor.OpenApertureBlock(blockId);
		processor.Move(origin);
		processor.PlotDraw(end);
		processor.Flash(origin);
		processor.CloseApertureBlock();
		processor.SetCurrentAperture(blockId);
	}
};

TEST(CommandsProcessor_ApertureBlock, CantCloseAgain) {
	CHECK_THROWS(std::logic_error, processor.CloseApertureBlock());
}

TEST(CommandsProcessor_ApertureBlock, DoesNotFlash) {
	LONGS_EQUAL(0, processor.GetObjects().size());
}

TEST(CommandsProcessor_ApertureBlock, AddedObjects) {
	std::shared_ptr<BlockAperture> block = GetAperture<BlockAperture>(processor,
			blockId);
	std::shared_ptr<Draw> draw = GetGraphicalObject<Draw>(*block->GetObjectList());

	LONGS_EQUAL(2, block->GetObjectList()->size());
	CHECK_EQUAL(origin, draw->GetSegment().GetStart());
}

TEST(CommandsProcessor_ApertureBlock, ClearsCurrentPoint) {
	CHECK(!processor.GetGraphicsState().GetCurrentPoint().has_value());
}

TEST(CommandsProcessor_ApertureBlock, FlashBlock) {
	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(
			processor.GetObjects());
	std::shared_ptr<BlockAperture> block = std::dynamic_pointer_cast<
			BlockAperture>(flash->GetAperture());

	CHECK(nullptr != block);
}

/***
 * Tests for Aperture Block, nested definitions
 */

TEST_GROUP(CommandsProcessor_NestedApertureBlock) {
	Point origin, end;
	CommandsProcessor processor;
	int outerBlockId, innerBlockId, circleId;

	void setup() {
		origin = Point(3000, -2000);
		end = Point(750, -500);
		circleId = 10;
		outerBlockId = 100;
		innerBlockId = 101;

		processor.GetGraphicsState().SetPlotState(PlotState::Linear);
		MakeAndSetAperture<Circle>(processor, circleId);
		processor.OpenApertureBlock(outerBlockId);
		processor.OpenApertureBlock(innerBlockId);
		processor.Move(origin);
		processor.PlotDraw(end);					//Plot to inner block
		processor.CloseApertureBlock();				//End inner block
		processor.SetCurrentAperture(innerBlockId);
		processor.Flash(origin);			//Flash inner block into outer block
		processor.CloseApertureBlock();				//End outer block
		processor.SetCurrentAperture(outerBlockId);
	}
};

TEST(CommandsProcessor_NestedApertureBlock, DoesNotFlash) {
	LONGS_EQUAL(0, processor.GetObjects().size());
}

TEST(CommandsProcessor_NestedApertureBlock, OuterContainsInner) {
	std::shared_ptr<BlockAperture> outerBlock = GetAperture<BlockAperture>(
			processor, outerBlockId);
	std::shared_ptr<BlockAperture> innerBlock = GetAperture<BlockAperture>(
			processor, innerBlockId);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(*outerBlock->GetObjectList());
	std::shared_ptr<BlockAperture> aperture = CheckAperture<BlockAperture>(
			*flash);
	CHECK_EQUAL(*innerBlock, *aperture);
}

TEST(CommandsProcessor_NestedApertureBlock, FlashBlock) {
	std::shared_ptr<BlockAperture> outerBlock = GetAperture<BlockAperture>(
			processor, outerBlockId);

	processor.Flash(origin);

	std::shared_ptr<Flash> flash = GetGraphicalObject<Flash>(
			processor.GetObjects());
	std::shared_ptr<BlockAperture> aperture = CheckAperture<BlockAperture>(
			*flash);
	CHECK_EQUAL(*outerBlock, *aperture);
}

/***
 * Tests for StepAndRepeat
 */

TEST_GROUP(CommandsProcessor_StepAndRepeat) {
	Point origin;
	CommandsProcessor processor;
	int nx, ny;

	void setup() {
		origin = Point(3000, -2000);
		nx = 2;
		ny = 3;

		MakeAndSetAperture<Circle>(processor, 10);
		processor.GetGraphicsState().SetPlotState(PlotState::Linear);
		processor.OpenStepAndRepeat(nx, ny, 0.5, 1.5);
		processor.Flash(origin);
		processor.CloseStepAndRepeat();
	}
};

TEST(CommandsProcessor_StepAndRepeat, CantOpenTwice) {
	processor.OpenStepAndRepeat(1, 1, 0.0, 0.0);
	CHECK_THROWS(std::logic_error, processor.OpenStepAndRepeat(1, 1, 0.0, 0.0));
}

TEST(CommandsProcessor_StepAndRepeat, CantCloseAgain) {
	CHECK_THROWS(std::logic_error, processor.CloseStepAndRepeat());
}

TEST(CommandsProcessor_StepAndRepeat, ClearsCurrentPoint) {
	CHECK(!processor.GetGraphicsState().GetCurrentPoint().has_value());
}

TEST(CommandsProcessor_StepAndRepeat, ExpandsObjects) {
	LONGS_EQUAL(nx * ny, processor.GetObjects().size());
}
