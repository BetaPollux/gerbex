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

#include "FileProcessor.h"
#include <fstream>
#include <sstream>
#include "CppUTest/TestHarness.h"

using namespace gerbex;

/**
 * Basics
 */

void loadfile(const std::string &path, FileProcessor &fileProcessor,
		CommandsProcessor **processor, GraphicsState **graphicsState) {
	//TODO make this path use a variable
	std::unique_ptr<std::istream> gerber = std::make_unique<std::ifstream>(path,
			std::ifstream::in);
	CHECK_TEXT(gerber->good(),
			"could not open Gerber file; run exec from build directory as ./bin/test_gerbex");

	fileProcessor.Process(std::move(gerber));
	*processor = &fileProcessor.GetProcessor();
	*graphicsState = &fileProcessor.GetProcessor().GetGraphicsState();
}

TEST_GROUP(GerberBasics) {
};

TEST(GerberBasics, ThrowsRuntimeError) {
	// Reaches EOF without closing delimiter
	std::unique_ptr<std::istream> gerber = std::make_unique<std::istringstream>(
			"%MOMM*\n");

	FileProcessor fileProcessor;
	CHECK_THROWS(std::runtime_error, fileProcessor.Process(std::move(gerber)));
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

TEST(GerberPolaritiesAndApertures, NotImplemented) {
	FAIL("GerberPolaritiesAndApertures not implemented");
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

TEST(GerberNestedBlocks, NotImplemented) {
	FAIL("GerberNestedBlocks not implemented");
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

TEST(GerberBlocksDiffOrientation, NotImplemented) {
	FAIL("GerberBlocksDiffOrientation not implemented");
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

TEST(GerberSampleMacro, NotImplemented) {
	FAIL("GerberSampleMacro not implemented");
}

/**
 * SMD Macro Prim 20
 */

TEST_GROUP(GerberMacroPrim20) {
	FileProcessor fileProcessor;
	CommandsProcessor *processor;
	GraphicsState *graphicsState;

	void setup() {
		loadfile("../Gerber_File_Format_Examples 20210409/SMD_prim_20_X1.gbr",
				fileProcessor, &processor, &graphicsState);
	}
};

TEST(GerberMacroPrim20, NotImplemented) {
	FAIL("GerberMacroPrim20 not implemented");
}

/**
 * SMD Macro Prim 21
 */

TEST_GROUP(GerberMacroPrim21) {
	FileProcessor fileProcessor;
	CommandsProcessor *processor;
	GraphicsState *graphicsState;

	void setup() {
		loadfile("../Gerber_File_Format_Examples 20210409/SMD_prim_21_X1.gbr",
				fileProcessor, &processor, &graphicsState);
	}
};

TEST(GerberMacroPrim21, NotImplemented) {
	FAIL("GerberMacroPrim21 not implemented");
}

