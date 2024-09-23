/*
 * test_CommandHandler.cpp
 *
 *  Created on: Sep. 15, 2024
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
#include <vector>

#include "CommandHandler.h"
#include "MockAperture.h"
#include "MockCommandsProcessor.h"
#include "MockTemplate.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace gerbex;

TEST_GROUP(CommandHandlerTest) {
	MockCommandsProcessor processor;
};

TEST(CommandHandlerTest, ApertureDefine) {
	std::shared_ptr<ApertureTemplate> tmpl = std::make_shared<MockTemplate>();
	std::unique_ptr<Aperture> aperture = std::make_unique<MockAperture>();
	std::vector<double> params = { 0.5, 0.25 };
	mock().expectOneCall("GetTemplate").withParameter("name", "C").andReturnValue(
			&tmpl);
	mock().expectOneCall("CallTemplate").withParameterOfType("Parameters",
			"parameters", &params).andReturnValue(aperture.get());
	mock().expectOneCall("ApertureDefine").withParameter("ident", 15).withParameter(
			"aperture", aperture.get());
	aperture.release();
	CommandHandler::ApertureDefine(processor, { "ADD15C,0.5X0.25" });
}

TEST(CommandHandlerTest, PlotDraw) {
	GraphicsState state;
	state.SetPlotState(PlotState::Linear);
	Point pt(200, 350);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("PlotDraw").withParameterOfType("Point", "coord", &pt);
	CommandHandler::Plot(processor, { "X200Y350D01" });
}

TEST(CommandHandlerTest, PlotArc) {
	GraphicsState state;
	state.SetPlotState(PlotState::Clockwise);
	Point pt(125, 475);
	Point os(50, -50);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("PlotArc").withParameterOfType("Point", "coord", &pt).withParameterOfType(
			"Point", "offset", &os);
	CommandHandler::Plot(processor, { "X125Y475I50J-50D01" });
}

TEST(CommandHandlerTest, RegionStatement_BeginRegion) {
	mock().expectOneCall("StartRegion");
	CommandHandler::RegionStatement(processor, { "G36" });
}

TEST(CommandHandlerTest, RegionStatement_EndRegion) {
	mock().expectOneCall("EndRegion");
	CommandHandler::RegionStatement(processor, { "G37" });
}

TEST(CommandHandlerTest, EndOfFile) {
	mock().expectOneCall("SetEndOfFile");
	CommandHandler::EndOfFile(processor, { "M02" });
}

TEST(CommandHandlerTest, Comment) {
	// Do nothing
	CommandHandler::Comment(processor, { "G04 My comment" });
}

TEST(CommandHandlerTest, Unit) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	CommandHandler::Unit(processor, { "MOIN" });
	CHECK(state.GetUnit() == gerbex::Unit::Inch);
}

TEST(CommandHandlerTest, Format) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	CommandHandler::Format(processor, { "FSLAX25Y25" });
	CHECK(state.GetFormat() == CoordinateFormat(2, 5));
}

TEST(CommandHandlerTest, ApertureMacro) {
	FAIL("Implemented handler for aperture macro");
}

TEST(CommandHandlerTest, PlotState) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	CommandHandler::PlotState(processor, { "G03" });
	CHECK(state.GetPlotState() == gerbex::PlotState::CounterClockwise);
}

TEST(CommandHandlerTest, Move) {
	GraphicsState state;
	Point pt(-100, 0);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Move").withParameterOfType("Point", "coord", &pt);
	CommandHandler::Move(processor, { "X-100Y0D02" });
}

TEST(CommandHandlerTest, Move_UsesCurrentPoint) {
	GraphicsState state;
	Point pt(1500, -2500);
	state.SetCurrentPoint(pt);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Move").withParameterOfType("Point", "coord", &pt);
	CommandHandler::Move(processor, { "D02" });
}

TEST(CommandHandlerTest, Flash) {
	GraphicsState state;
	Point pt(-100, 0);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Flash").withParameterOfType("Point", "coord", &pt);
	CommandHandler::Flash(processor, { "X-100Y0D03" });
}

TEST(CommandHandlerTest, Flash_UsesCurrentPoint) {
	GraphicsState state;
	Point pt(1500, -2500);
	state.SetCurrentPoint(pt);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Flash").withParameterOfType("Point", "coord", &pt);
	CommandHandler::Flash(processor, { "D03" });
}

TEST(CommandHandlerTest, Polarity) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	CommandHandler::ApertureTransformations(processor, { "LPC" });
	CHECK(state.GetTransformation().GetPolarity() == Polarity::Clear);
}

TEST(CommandHandlerTest, Mirroring) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	CommandHandler::ApertureTransformations(processor, { "LMXY" });
	CHECK(state.GetTransformation().GetMirroring() == Mirroring::XY);
}

TEST(CommandHandlerTest, Rotation) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	CommandHandler::ApertureTransformations(processor, { "LR-22.5" });
	CHECK(state.GetTransformation().GetRotationDegrees() == -22.5);
}

TEST(CommandHandlerTest, Scaling) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	CommandHandler::ApertureTransformations(processor, { "LS0.4" });
	CHECK(state.GetTransformation().GetScalingFactor() == 0.4);
}

TEST(CommandHandlerTest, BlockAperture_open) {
	mock().expectOneCall("OpenApertureBlock").withParameter("ident", 255);
	CommandHandler::BlockAperture(processor, { "ABD255" });
}

TEST(CommandHandlerTest, BlockAperture_close) {
	mock().expectOneCall("CloseApertureBlock");
	CommandHandler::BlockAperture(processor, { "AB" });
}

TEST(CommandHandlerTest, StepAndRepeat_open) {
	mock().expectOneCall("OpenStepAndRepeat").withParameter("nx", 4).withParameter(
			"ny", 1).withParameter("dx", 5.0).withParameter("dy", 0.0);
	CommandHandler::StepAndRepeat(processor, { "SRX4Y1I5.0J0" });
}

TEST(CommandHandlerTest, StepAndRepeat_close) {
	mock().expectOneCall("CloseStepAndRepeat");
	CommandHandler::StepAndRepeat(processor, { "SR" });
}

