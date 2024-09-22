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
	Point pt0;
	Point pt(200, 350);
	mock().expectOneCall("GetCurrentPoint").andReturnValue(&pt0);
	mock().expectOneCall("GetPlotState").andReturnValue(
			(int) PlotState::Linear);
	mock().expectOneCall("PlotDraw").withParameterOfType("Point", "coord", &pt);
	CommandHandler::Plot(processor, { "X200Y350D01" });
}

TEST(CommandHandlerTest, RegionStatement_BeginRegion) {
	mock().expectOneCall("StartRegion");
	CommandHandler::RegionStatement(processor, { "G36" });
}

TEST(CommandHandlerTest, RegionStatement_EndRegion) {
	mock().expectOneCall("EndRegion");
	CommandHandler::RegionStatement(processor, { "G37" });
}

