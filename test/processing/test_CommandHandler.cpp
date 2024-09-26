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
		std::list<std::string> words, std::vector<double> params,
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
	state.SetPlotState(PlotState::Linear);
	Point pt(200, 350);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("PlotDraw").withParameterOfType("Point", "coord", &pt);
	std::list<std::string> words = { "X200Y350D01" };
	CommandHandler::Plot(processor, words);
}

TEST(CommandHandlerTest, PlotArc) {
	GraphicsState state;
	state.SetPlotState(PlotState::Clockwise);
	Point pt(125, 475);
	Point os(50, -50);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("PlotArc").withParameterOfType("Point", "coord", &pt).withParameterOfType(
			"Point", "offset", &os);
	std::list<std::string> words = { "X125Y475I50J-50D01" };
	CommandHandler::Plot(processor, words);
}

TEST(CommandHandlerTest, RegionStatement_BeginRegion) {
	mock().expectOneCall("StartRegion");
	std::list<std::string> words = { "G36" };
	CommandHandler::RegionStatement(processor, words);
}

TEST(CommandHandlerTest, RegionStatement_EndRegion) {
	mock().expectOneCall("EndRegion");
	std::list<std::string> words = { "G37" };
	CommandHandler::RegionStatement(processor, words);
}

TEST(CommandHandlerTest, EndOfFile) {
	mock().expectOneCall("SetEndOfFile");
	std::list<std::string> words = { "M02" };
	CommandHandler::EndOfFile(processor, words);
}

TEST(CommandHandlerTest, Comment) {
	// Do nothing
	std::list<std::string> words = { "G04 My comment" };
	CommandHandler::Comment(processor, words);
}

TEST(CommandHandlerTest, Unit) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	std::list<std::string> words = { "MOIN" };
	CommandHandler::Unit(processor, words);
	CHECK(state.GetUnit() == gerbex::Unit::Inch);
}

TEST(CommandHandlerTest, Format) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	std::list<std::string> words = { "FSLAX25Y25" };
	CommandHandler::Format(processor, words);
	CHECK(state.GetFormat() == CoordinateFormat(2, 5));
}

TEST(CommandHandlerTest, ApertureMacro) {
	std::list<std::string> words = { "AMTHERMAL80", "1,1,$1,$2,$3",
			"1,0,$4,$2,$3" };
	std::list<std::string> body = std::list<std::string>(++words.begin(),
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
	std::list<std::string> words = { "G03" };
	CommandHandler::PlotState(processor, words);
	CHECK(state.GetPlotState() == gerbex::PlotState::CounterClockwise);
}

TEST(CommandHandlerTest, Move) {
	GraphicsState state;
	Point pt(-100, 0);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Move").withParameterOfType("Point", "coord", &pt);
	std::list<std::string> words = { "X-100Y0D02" };
	CommandHandler::Move(processor, words);
}

TEST(CommandHandlerTest, Move_UsesCurrentPoint) {
	GraphicsState state;
	Point pt(1500, -2500);
	state.SetCurrentPoint(pt);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Move").withParameterOfType("Point", "coord", &pt);
	std::list<std::string> words = { "D02" };
	CommandHandler::Move(processor, words);
}

TEST(CommandHandlerTest, Flash) {
	GraphicsState state;
	Point pt(-100, 0);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Flash").withParameterOfType("Point", "coord", &pt);
	std::list<std::string> words = { "X-100Y0D03" };
	CommandHandler::Flash(processor, words);
}

TEST(CommandHandlerTest, Flash_UsesCurrentPoint) {
	GraphicsState state;
	Point pt(1500, -2500);
	state.SetCurrentPoint(pt);
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	mock().expectOneCall("Flash").withParameterOfType("Point", "coord", &pt);
	std::list<std::string> words = { "D03" };
	CommandHandler::Flash(processor, words);
}

TEST(CommandHandlerTest, Polarity) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	std::list<std::string> words = { "LPC" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetPolarity() == Polarity::Clear);
}

TEST(CommandHandlerTest, Mirroring) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	std::list<std::string> words = { "LMXY" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetMirroring() == Mirroring::XY);
}

TEST(CommandHandlerTest, Rotation) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	std::list<std::string> words = { "LR-22.5" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetRotationDegrees() == -22.5);
}

TEST(CommandHandlerTest, Scaling) {
	GraphicsState state;
	mock().expectOneCall("GetGraphicsState").andReturnValue(&state);
	std::list<std::string> words = { "LS0.4" };
	CommandHandler::ApertureTransformations(processor, words);
	CHECK(state.GetTransformation().GetScalingFactor() == 0.4);
}

TEST(CommandHandlerTest, BlockAperture_open) {
	mock().expectOneCall("OpenApertureBlock").withParameter("ident", 255);
	std::list<std::string> words = { "ABD255" };
	CommandHandler::BlockAperture(processor, words);
}

TEST(CommandHandlerTest, BlockAperture_close) {
	mock().expectOneCall("CloseApertureBlock");
	std::list<std::string> words = { "AB" };
	CommandHandler::BlockAperture(processor, words);
}

TEST(CommandHandlerTest, StepAndRepeat_open) {
	mock().expectOneCall("OpenStepAndRepeat").withParameter("nx", 4).withParameter(
			"ny", 1).withParameter("dx", 5.0).withParameter("dy", 0.0);
	std::list<std::string> words = { "SRX4Y1I5.0J0" };
	CommandHandler::StepAndRepeat(processor, words);
}

TEST(CommandHandlerTest, StepAndRepeat_close) {
	mock().expectOneCall("CloseStepAndRepeat");
	std::list<std::string> words = { "SR" };
	CommandHandler::StepAndRepeat(processor, words);
}

