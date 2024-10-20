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
#include "MacroTemplate.h"
#include "MockAperture.h"
#include "MockCommandsProcessor.h"
#include "MockTemplate.h"
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

using namespace gerbex;

TEST_GROUP(CommandHandlerTest) {
	MockCommandsProcessor processor;
};

void test_ApertureDefine(MockCommandsProcessor &processor,
		Fields words, Parameters params,
		std::string name, int ident) {
	std::shared_ptr<ApertureTemplate> tmpl = std::make_shared<MockTemplate>();
	std::unique_ptr<Aperture> aperture = std::make_unique<MockAperture>();
	mock().expectOneCall("GetTemplate").withParameter("name", name.c_str()).andReturnValue(
			&tmpl);
	mock().expectOneCall("CallTemplate").withParameterOfType("Parameters",
			"parameters", &params).andReturnValue(aperture.get());
	mock().expectOneCall("ApertureDefine").withParameter("ident", ident).withParameter(
			"aperture", aperture.get());
	aperture.release();
	CommandHandler::ApertureDefine(processor, words);
}

TEST(CommandHandlerTest, ApertureDefine) {
	test_ApertureDefine(processor, { "ADD15C,0.5X0.25" }, { 0.5, 0.25 }, "C",
			15);
}

TEST(CommandHandlerTest, ApertureDefine_NoParams) {
	test_ApertureDefine(processor, { "ADD19THERMAL80" }, { }, "THERMAL80", 19);
}

TEST(CommandHandlerTest, PlotDraw) {
	GraphicsState state;
	state.SetFormat(CoordinateFormat());
	state.SetPlotState(PlotState::Linear);
	Point pt(2.0, 3.5);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("PlotDraw").withParameterOfType("Point", "coord", &pt);
	Fields words = { "X2000000Y3500000D01" };
	CommandHandler::Plot(processor, words);
}

TEST(CommandHandlerTest, PlotArc) {
	GraphicsState state;
	state.SetFormat(CoordinateFormat());
	state.SetPlotState(PlotState::Clockwise);
	state.SetArcMode(ArcMode::MultiQuadrant);
	Point pt(0.125, 0.475);
	Point os(0.500, -0.500);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("PlotArc").withParameterOfType("Point", "coord", &pt).withParameterOfType(
			"Point", "offset", &os);
	Fields words = { "X125000Y475000I500000J-500000D01" };
	CommandHandler::Plot(processor, words);
}

TEST(CommandHandlerTest, RegionStatement_BeginRegion) {
	mock().expectOneCall("StartRegion");
	Fields words = { "G36" };
	CommandHandler::RegionStatement(processor, words);
}

TEST(CommandHandlerTest, RegionStatement_EndRegion) {
	mock().expectOneCall("EndRegion");
	Fields words = { "G37" };
	CommandHandler::RegionStatement(processor, words);
}

TEST(CommandHandlerTest, EndOfFile) {
	mock().expectOneCall("SetEndOfFile");
	Fields words = { "M02" };
	CommandHandler::EndOfFile(processor, words);
}

TEST(CommandHandlerTest, Comment) {
	// Do nothing
	Fields words = { "G04 My comment" };
	CommandHandler::Comment(processor, words);
}

TEST(CommandHandlerTest, Unit) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	Fields words = { "MOIN" };
	CommandHandler::Unit(processor, words);
	CHECK(state.GetUnit() == gerbex::Unit::Inch);
}

TEST(CommandHandlerTest, Format) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	Fields words = { "FSLAX25Y25" };
	CommandHandler::Format(processor, words);
	CHECK(state.GetFormat() == CoordinateFormat(2, 5));
}

TEST(CommandHandlerTest, ApertureMacro) {
	Fields words = { "AMTHERMAL80", "1,1,$1,$2,$3",
			"1,0,$4,$2,$3" };
	Fields body = Fields(++words.begin(),
			words.end());
	std::shared_ptr<MacroTemplate> macro = std::make_shared<MacroTemplate>(
			body);
	mock().expectOneCall("AddTemplate").withParameter("name", "THERMAL80").withParameterOfType(
			"MacroTemplate", "new_tmpl", macro.get());
	CommandHandler::ApertureMacro(processor, words);
}

TEST(CommandHandlerTest, PlotState) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	Fields words = { "G03" };
	CommandHandler::PlotState(processor, words);
	CHECK(state.GetPlotState() == gerbex::PlotState::CounterClockwise);
}

TEST(CommandHandlerTest, Move) {
	GraphicsState state;
	state.SetFormat(CoordinateFormat());
	Point pt(-1.0, 0);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Move").withParameterOfType("Point", "coord", &pt);
	Fields words = { "X-1000000Y0D02" };
	CommandHandler::Move(processor, words);
}

TEST(CommandHandlerTest, Move_UsesCurrentPoint) {
	GraphicsState state;
	state.SetFormat(CoordinateFormat());
	Point pt(1500, -2500);
	state.SetCurrentPoint(pt);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Move").withParameterOfType("Point", "coord", &pt);
	Fields words = { "D02" };
	CommandHandler::Move(processor, words);
}

TEST(CommandHandlerTest, Flash) {
	GraphicsState state;
	state.SetFormat(CoordinateFormat());
	Point pt(-1.0, 0);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Flash").withParameterOfType("Point", "coord", &pt);
	Fields words = { "X-1000000Y0D03" };
	CommandHandler::Flash(processor, words);
}

TEST(CommandHandlerTest, Flash_UsesCurrentPoint) {
	GraphicsState state;
	state.SetFormat(CoordinateFormat());
	Point pt(1500, -2500);
	state.SetCurrentPoint(pt);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Flash").withParameterOfType("Point", "coord", &pt);
	Fields words = { "D03" };
	CommandHandler::Flash(processor, words);
}

TEST(CommandHandlerTest, Polarity) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	Fields words = { "LPC" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetPolarity() == Polarity::Clear);
}

TEST(CommandHandlerTest, Mirroring) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	Fields words = { "LMXY" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetMirroring() == Mirroring::XY);
}

TEST(CommandHandlerTest, Rotation) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	Fields words = { "LR-22.5" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetRotation() == -22.5);
}

TEST(CommandHandlerTest, Scaling) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	Fields words = { "LS0.4" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetScaling() == 0.4);
}

TEST(CommandHandlerTest, BlockAperture_open) {
	mock().expectOneCall("OpenApertureBlock").withParameter("ident", 255);
	Fields words = { "ABD255" };
	CommandHandler::BlockAperture(processor, words);
}

TEST(CommandHandlerTest, BlockAperture_close) {
	mock().expectOneCall("CloseApertureBlock");
	Fields words = { "AB" };
	CommandHandler::BlockAperture(processor, words);
}

TEST(CommandHandlerTest, StepAndRepeat_open) {
	mock().expectOneCall("OpenStepAndRepeat").withParameter("nx", 4).withParameter(
			"ny", 1).withParameter("dx", 5.0).withParameter("dy", 0.0);
	Fields words = { "SRX4Y1I5.0J0" };
	CommandHandler::StepAndRepeat(processor, words);
}

TEST(CommandHandlerTest, StepAndRepeat_close) {
	mock().expectOneCall("CloseStepAndRepeat");
	Fields words = { "SR" };
	CommandHandler::StepAndRepeat(processor, words);
}

