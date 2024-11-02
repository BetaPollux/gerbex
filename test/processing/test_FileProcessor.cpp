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

#include "Arc.h"
#include "BlockAperture.h"
#include "CastHelpers.h"
#include "Draw.h"
#include "FileProcessor.h"
#include "Flash.h"
#include "Macro.h"
#include "MacroCenterLine.h"
#include "MacroCircle.h"
#include "MacroTemplate.h"
#include "MacroThermal.h"
#include <fstream>
#include <sstream>
#include "CppUTest/TestHarness.h"
#include "../graphics/GraphicsTestHelpers.h"

using namespace gerbex;

/**
 * Basics
 */

void loadfile(const std::string &path, FileProcessor &fileProcessor,
		CommandsProcessor **processor, GraphicsState **graphicsState) {
	//TODO make this path use a variable
	std::ifstream gerber = std::ifstream(path, std::ifstream::in);
	CHECK_TEXT(gerber.good(),
			"could not open Gerber file; run exec from build directory as ./bin/test_gerbex");

	fileProcessor.Process(gerber);
	*processor = &fileProcessor.GetProcessor();
	*graphicsState = &fileProcessor.GetProcessor().GetGraphicsState();
}

TEST_GROUP(GerberBasics) {
};

TEST(GerberBasics, ThrowsRuntimeError) {
	// Reaches EOF without closing delimiter
	std::istringstream gerber = std::istringstream("%MOMM*\n");

	FileProcessor fileProcessor;
	CHECK_THROWS(std::runtime_error, fileProcessor.Process(gerber));
}

/**
 * Two Square Boxes
 */

TEST_GROUP(GerberTwoSquareBoxes) {
	FileProcessor fileProcessor;
	CommandsProcessor *processor;
	GraphicsState *graphicsState;

	void setup() {
		loadfile(
				"../Gerber_File_Format_Examples 20210409/2-13-1_Two_square_boxes.gbr",
				fileProcessor, &processor, &graphicsState);
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
	CHECK(Polarity::Dark == graphicsState->GetPolarity());
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
	CHECK(graphicsState->GetCurrentPoint().has_value());
	CHECK_EQUAL(Point(6.0, 0), *graphicsState->GetCurrentPoint());
}

TEST(GerberTwoSquareBoxes, EndOfFile) {
	CHECK(CommandState::EndOfFile == processor->GetCommandState());
}

/**
 * Polarities and Apertures
 */

TEST_GROUP(GerberPolaritiesAndApertures) {
	FileProcessor fileProcessor;
	CommandsProcessor *processor;
	GraphicsState *graphicsState;

	void setup() {
		loadfile(
				"../Gerber_File_Format_Examples 20210409/2-13-2_Polarities_and_Apertures.gbr",
				fileProcessor, &processor, &graphicsState);
	}
};

TEST(GerberPolaritiesAndApertures, ReadAndUsedMacro) {
	std::shared_ptr<ApertureTemplate> templ = processor->GetTemplate(
			"THERMAL80");
	std::shared_ptr<Macro> macro = GetAperture<Macro>(*processor, 19);
	std::shared_ptr<MacroThermal> thermal = GetMacroPrimitive<MacroThermal>(
			*macro);

	MacroThermal expected(Point(0.0, 0.0), 0.800, 0.550, 0.125, 45.0);
	CHECK(expected == *thermal);
}

TEST(GerberPolaritiesAndApertures, TwoRegionsWithPolarity) {
	std::shared_ptr<Region> outer_region = GetGraphicalObject<Region>(
			processor->GetObjects(), 17);
	std::shared_ptr<Region> inner_region = GetGraphicalObject<Region>(
			processor->GetObjects(), 18);

	LONGS_EQUAL(4, outer_region->GetContours().front().GetSegments().size());
	CHECK(outer_region->GetPolarity() == Polarity::Dark);
	LONGS_EQUAL(5, inner_region->GetContours().front().GetSegments().size());
	CHECK(inner_region->GetPolarity() == Polarity::Clear);
}

/**
 * Nested Blocks
 */

TEST_GROUP(GerberNestedBlocks) {
	FileProcessor fileProcessor;
	CommandsProcessor *processor;
	GraphicsState *graphicsState;

	void setup() {
		loadfile(
				"../Gerber_File_Format_Examples 20210409/4-6-4_Nested_blocks.gbr",
				fileProcessor, &processor, &graphicsState);
	}
};

TEST(GerberNestedBlocks, MadeAllBlocks) {
	std::shared_ptr<BlockAperture> b100 = GetAperture<BlockAperture>(*processor,
			100);
	std::shared_ptr<BlockAperture> b101 = GetAperture<BlockAperture>(*processor,
			101);
	std::shared_ptr<BlockAperture> b102 = GetAperture<BlockAperture>(*processor,
			102);

	LONGS_EQUAL(3, b100->GetObjectList()->size());
	LONGS_EQUAL(4, b101->GetObjectList()->size());
	LONGS_EQUAL(7, b102->GetObjectList()->size());

	auto flash1 = GetGraphicalObject<Flash>(*b101->GetObjectList());
	auto flash2 = GetGraphicalObject<Flash>(*b102->GetObjectList());
	auto nb100 = CheckAperture<BlockAperture>(*flash1);
	auto nb101 = CheckAperture<BlockAperture>(*flash2);
	CHECK_EQUAL(*b100, *nb100);
	CHECK_EQUAL(*b101, *nb101);
}

/**
 * Block with Different Orientations
 */

TEST_GROUP(GerberBlocksDiffOrientation) {
	FileProcessor fileProcessor;
	CommandsProcessor *processor;
	GraphicsState *graphicsState;

	void setup() {
		loadfile(
				"../Gerber_File_Format_Examples 20210409/4-11-6_Block_with_different_orientations.gbr",
				fileProcessor, &processor, &graphicsState);
	}
};

TEST(GerberBlocksDiffOrientation, MadeBlock) {
	std::shared_ptr<BlockAperture> block = GetAperture<BlockAperture>(
			*processor, 12);
	std::shared_ptr<Flash> c1 = GetGraphicalObject<Flash>(
			*block->GetObjectList(), 0);
	std::shared_ptr<Flash> c2 = GetGraphicalObject<Flash>(
			*block->GetObjectList(), 1);
	std::shared_ptr<Flash> c3 = GetGraphicalObject<Flash>(
			*block->GetObjectList(), 2);
	std::shared_ptr<Draw> d1 = GetGraphicalObject<Draw>(*block->GetObjectList(),
			3);
	std::shared_ptr<Arc> a1 = GetGraphicalObject<Arc>(*block->GetObjectList(),
			4);

	CHECK_EQUAL(Point(-0.5, -1.0), d1->GetSegment().GetStart());
	CHECK_EQUAL(Point(2.5, -1.0), d1->GetSegment().GetEnd());

	CHECK(ArcDirection::CounterClockwise == a1->GetSegment().GetDirection());
}

TEST(GerberBlocksDiffOrientation, FlashedFourTimes) {
	std::shared_ptr<Flash> b1 = GetGraphicalObject<Flash>(
			processor->GetObjects(), 0);
	std::shared_ptr<Flash> b2 = GetGraphicalObject<Flash>(
			processor->GetObjects(), 1);
	std::shared_ptr<Flash> b3 = GetGraphicalObject<Flash>(
			processor->GetObjects(), 2);
	std::shared_ptr<Flash> b4 = GetGraphicalObject<Flash>(
			processor->GetObjects(), 3);
}

/**
 * Sample Macro
 */

TEST_GROUP(GerberSampleMacro) {
	FileProcessor fileProcessor;
	CommandsProcessor *processor;
	GraphicsState *graphicsState;

	void setup() {
		loadfile("../Gerber_File_Format_Examples 20210409/sample_macro_X1.gbr",
				fileProcessor, &processor, &graphicsState);
	}
};

TEST(GerberSampleMacro, MadeMacros) {
	processor->GetTemplate("BOXR");
	processor->GetTemplate("BOXS");
	processor->GetTemplate("BOXS2");
	processor->GetTemplate("THERS4T");
	processor->GetTemplate("THERR4");
	processor->GetTemplate("DONSS");
	processor->GetTemplate("DONSR");
}

TEST(GerberSampleMacro, BOXR_D12) {
	std::shared_ptr<Macro> boxr_d12 = GetAperture<Macro>(*processor, 12);
	std::shared_ptr<MacroCenterLine> s1 = GetMacroPrimitive<MacroCenterLine>(
			*boxr_d12, 0);
	std::shared_ptr<MacroCircle> c4 = GetMacroPrimitive<MacroCircle>(*boxr_d12,
			5);

	MacroCenterLine expectedLine(MacroExposure::ON, 0.2550, 0.1 - 2 * 0.02,
			Point(0.0, 0.0), 30.0);
	MacroCircle expectedCircle(MacroExposure::ON, 2 * 0.02,
			Point(0.2550 / 2.0 - 0.02, -(-0.02 + 0.1 / 2.0)), 30.0);
	CHECK(expectedLine == *s1);
	CHECK(expectedCircle == *c4);
}
