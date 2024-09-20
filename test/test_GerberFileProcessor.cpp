/*
 * test_GerberFileProcessor.cpp
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

#include <sstream>

#include "GerberFileProcessor.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(GerberBasics) {
};

TEST(GerberBasics, ThrowsRuntimeError) {
	// Reaches EOF without closing delimiter
	std::unique_ptr<std::istream> gerber = std::make_unique<std::istringstream>("%MOMM*\n");
	GerberFileProcessor fileProcessor;
	CHECK_THROWS(std::runtime_error, fileProcessor.Process(std::move(gerber)));
}

TEST_GROUP(GerberTwoSquareBoxes) {
	GerberFileProcessor fileProcessor;
	CommandsProcessor *processor;
	const GraphicsState *graphicsState;

	void setup() {
		std::unique_ptr<std::istream> gerber = std::make_unique<std::istringstream>(
			"G04 Ucamco ex. 1: Two square boxes*\n"
			"%FSLAX26Y26*%\n"
			"%MOMM*%\n"
			"%TF.Part,Other,example*%\n"
			"%LPD*%\n"
			"%ADD10C,0.010*%\n"
			"D10*\n"
			"X0Y0D02*\n"
			"G01*\n"
			"X5000000Y0D01*\n"
			"Y5000000D01*\n"
			"X0D01*\n"
			"Y0D01*\n"
			"X6000000D02*\n"
			"X11000000D01*\n"
			"Y5000000D01*\n"
			"X6000000D01*\n"
			"Y0D01*\n"
			"M02*\n");
		
		fileProcessor.Process(std::move(gerber));
		processor = &fileProcessor.GetProcessor();
		graphicsState = &processor->GetGraphicsState();
	}
};

TEST(GerberTwoSquareBoxes, SetFormat) {
	CHECK(graphicsState->GetFormat().has_value());
	LONGS_EQUAL(2, graphicsState->GetFormat()->GetInteger());
	LONGS_EQUAL(6, graphicsState->GetFormat()->GetDecimal());
}

TEST(GerberTwoSquareBoxes, SetUnits) {
	CHECK(Unit::Millimeter == graphicsState->GetUnit());
}

TEST(GerberTwoSquareBoxes, SetPolarity) {
	CHECK(Polarity::Dark == processor->GetTransformation().GetPolarity());
}

TEST(GerberTwoSquareBoxes, DefinedAperture) {
	auto aperture = processor->GetAperture(10);
	CHECK(aperture != nullptr);
}

TEST(GerberTwoSquareBoxes, SetPlotState) {
	CHECK(PlotState::Linear == graphicsState->GetPlotState());
}

TEST(GerberTwoSquareBoxes, Draws) {
	auto objects = processor->GetObjects();
	LONGS_EQUAL(8, objects.size());
}

TEST(GerberTwoSquareBoxes, LastPoint) {
	CHECK(Point(6000000, 0) == graphicsState->GetCurrentPoint());
}

TEST(GerberTwoSquareBoxes, EndOfFile) {
	CHECK(CommandState::EndOfFile == processor->GetCommandState());
}
