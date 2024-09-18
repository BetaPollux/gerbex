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

TEST_GROUP(GerberTwoSquareBoxes) {
	GerberFileProcessor fileProcessor;
	const CommandsProcessor *processor;
	const GraphicsState *graphicsState;

	void setup() {
		std::unique_ptr<std::istream> gerber = std::make_unique<std::istringstream>(
			"G04 Ucamco ex. 1: Two square boxes*"
			"%FSLAX26Y26*%"
			"%MOMM*%"
			"%TF.Part,Other,example*%"
			"%LPD*%"
			"%ADD10C,0.010*%"
			"D10*"
			"X0Y0D02*"
			"G01*"
			"X5000000Y0D01*"
			"Y5000000D01*"
			"X0D01*"
			"Y0D01*"
			"X6000000D02*"
			"X11000000D01*"
			"Y5000000D01*"
			"X6000000D01*"
			"Y0D01*"
			"M02*");
		
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
	CHECK(Polarity::Dark == graphicsState->GetTransformation().GetPolarity());
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
