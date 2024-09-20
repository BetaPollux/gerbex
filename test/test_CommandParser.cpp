/*
 * test_CommandParser.cpp
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

#include "CommandParser.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(GetCommandCode) {
};


TEST(GetCommandCode, G04) {
	STRCMP_EQUAL("G04", CommandParser::GetCommandCode("G04 Comment").c_str());
}

TEST(GetCommandCode, MO) {
	STRCMP_EQUAL("MO", CommandParser::GetCommandCode("MOMM").c_str());
}

TEST(GetCommandCode, FS) {
	STRCMP_EQUAL("FS", CommandParser::GetCommandCode("FSLAX26Y26").c_str());
}

TEST(GetCommandCode, AD) {
	STRCMP_EQUAL("AD", CommandParser::GetCommandCode("ADD10C,0.010").c_str());
}

TEST(GetCommandCode, AM) {
	STRCMP_EQUAL("AM", CommandParser::GetCommandCode("AMTHERMAL80").c_str());
}

TEST(GetCommandCode, Dnn) {
	STRCMP_EQUAL("Dnn", CommandParser::GetCommandCode("D10").c_str());
	STRCMP_EQUAL("Dnn", CommandParser::GetCommandCode("D99").c_str());
	STRCMP_EQUAL("Dnn", CommandParser::GetCommandCode("D2147483647").c_str());
}

TEST(GetCommandCode, PlotState) {
	STRCMP_EQUAL("G75", CommandParser::GetCommandCode("G75").c_str());
	STRCMP_EQUAL("G01", CommandParser::GetCommandCode("G01").c_str());
	STRCMP_EQUAL("G02", CommandParser::GetCommandCode("G02").c_str());
	STRCMP_EQUAL("G03", CommandParser::GetCommandCode("G03").c_str());
}

TEST(GetCommandCode, Operations) {
	std::vector<std::string> opcodes = { "D01", "D02", "D03" };
	for (std::string &op : opcodes) {
		std::vector<std::string> words = { "X250000Y155000", "Y155000",
				"X250000", "", "X75000Y50000I40000J0", "I40000J0" };
		for (std::string &w : words) {
			STRCMP_EQUAL(op.c_str(), CommandParser::GetCommandCode(w + op).c_str());
		}
	}
}

TEST(GetCommandCode, ApertureTransformations) {
	STRCMP_EQUAL("LP", CommandParser::GetCommandCode("LPC").c_str());
	STRCMP_EQUAL("LM", CommandParser::GetCommandCode("LMN").c_str());
	STRCMP_EQUAL("LR", CommandParser::GetCommandCode("LR90").c_str());
	STRCMP_EQUAL("LS", CommandParser::GetCommandCode("LS0.8").c_str());
}

TEST(GetCommandCode, RegionStatement) {
	STRCMP_EQUAL("G36", CommandParser::GetCommandCode("G36").c_str());
	STRCMP_EQUAL("G37", CommandParser::GetCommandCode("G37").c_str());
}

TEST(GetCommandCode, BlockAperture) {
	STRCMP_EQUAL("AB", CommandParser::GetCommandCode("ABD12").c_str());
	STRCMP_EQUAL("AB", CommandParser::GetCommandCode("AB").c_str());
}

TEST(GetCommandCode, StepAndRepeat) {
	STRCMP_EQUAL("SR", CommandParser::GetCommandCode("SRX2Y3I2.0J3.0").c_str());
	STRCMP_EQUAL("SR", CommandParser::GetCommandCode("SR").c_str());
}

TEST(GetCommandCode, EndOfFile) {
	STRCMP_EQUAL("M02", CommandParser::GetCommandCode("M02").c_str());
}

TEST(GetCommandCode, Attributes) {
	STRCMP_EQUAL("TF", CommandParser::GetCommandCode("TFMyAttribute,Yes").c_str());
	STRCMP_EQUAL("TA", CommandParser::GetCommandCode("TA.AperFunction,ComponentPad").c_str());
	STRCMP_EQUAL("TO", CommandParser::GetCommandCode("TO.C,R6").c_str());
	STRCMP_EQUAL("TD", CommandParser::GetCommandCode("TD.AperFunction").c_str());
}

TEST(GetCommandCode, Unknown) {
	CHECK_THROWS(std::invalid_argument, CommandParser::GetCommandCode("Z"));
}

//TODO test handlers

