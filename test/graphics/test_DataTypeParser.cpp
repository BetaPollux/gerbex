/*
 * test_DataTypeDataTypeParser::cpp
 *
 *  Created on: Sep. 17, 2024
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

#include <regex>
#include <stdexcept>
#include <vector>

#include "DataTypeParser.h"
#include "CppUTest/TestHarness.h"

using namespace gerbex;


#define DBL_TOL 1e-5

TEST_GROUP(DataTypeParserTest) {
};

TEST(DataTypeParserTest, Number) {
	STRCMP_EQUAL("0", DataTypeParser::Match("0", DataTypeParser::GetNumberPattern()).c_str());
	STRCMP_EQUAL("-1024", DataTypeParser::Match("-1024", DataTypeParser::GetNumberPattern()).c_str());
	STRCMP_EQUAL("+16", DataTypeParser::Match("+16", DataTypeParser::GetNumberPattern()).c_str());
	STRCMP_EQUAL("-200",  DataTypeParser::Match("-200", DataTypeParser::GetNumberPattern()).c_str());
	STRCMP_EQUAL("1234.56", DataTypeParser::Match("1234.56", DataTypeParser::GetNumberPattern()).c_str());
	STRCMP_EQUAL(".123", DataTypeParser::Match(".123", DataTypeParser::GetNumberPattern()).c_str());
	STRCMP_EQUAL("-0.128", DataTypeParser::Match("-0.128", DataTypeParser::GetNumberPattern()).c_str());
}

TEST(DataTypeParserTest, Field) {
	STRCMP_EQUAL("", DataTypeParser::Match("", DataTypeParser::GetFieldPattern()).c_str());
	STRCMP_EQUAL("2", DataTypeParser::Match("2", DataTypeParser::GetFieldPattern()).c_str());
	STRCMP_EQUAL("123ABC", DataTypeParser::Match("123ABC", DataTypeParser::GetFieldPattern()).c_str());
	STRCMP_EQUAL("A&B", DataTypeParser::Match("A&B", DataTypeParser::GetFieldPattern()).c_str());
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("ABC,123", DataTypeParser::GetFieldPattern()));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("ABC*", DataTypeParser::GetFieldPattern()));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("%ABC", DataTypeParser::GetFieldPattern()));
}

TEST(DataTypeParserTest, Name) {
	STRCMP_EQUAL(".ABC", DataTypeParser::Match(".ABC", DataTypeParser::GetNamePattern()).c_str());
	STRCMP_EQUAL("$3", DataTypeParser::Match("$3", DataTypeParser::GetNamePattern()).c_str());
	STRCMP_EQUAL(".3abc", DataTypeParser::Match(".3abc", DataTypeParser::GetNamePattern()).c_str());
	STRCMP_EQUAL("ABC.123", DataTypeParser::Match("ABC.123", DataTypeParser::GetNamePattern()).c_str());
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("", DataTypeParser::GetNamePattern()));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("3", DataTypeParser::GetNamePattern()));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("A&B", DataTypeParser::GetNamePattern()));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("ABC,123", DataTypeParser::GetNamePattern()));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("%ABC", DataTypeParser::GetNamePattern()));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Match("ABC*", DataTypeParser::GetNamePattern()));
}


/**
 * Get Command Code
 */


TEST_GROUP(GetCommandCode) {
};


TEST(GetCommandCode, G04) {
	STRCMP_EQUAL("G04", DataTypeParser::GetCommandCode("G04 Comment").c_str());
}

TEST(GetCommandCode, MO) {
	STRCMP_EQUAL("MO", DataTypeParser::GetCommandCode("MOMM").c_str());
}

TEST(GetCommandCode, FS) {
	STRCMP_EQUAL("FS", DataTypeParser::GetCommandCode("FSLAX26Y26").c_str());
}

TEST(GetCommandCode, AD) {
	STRCMP_EQUAL("AD", DataTypeParser::GetCommandCode("ADD10C,0.010").c_str());
}

TEST(GetCommandCode, AM) {
	STRCMP_EQUAL("AM", DataTypeParser::GetCommandCode("AMTHERMAL80").c_str());
}

TEST(GetCommandCode, Dnn) {
	STRCMP_EQUAL("Dnn", DataTypeParser::GetCommandCode("D10").c_str());
	STRCMP_EQUAL("Dnn", DataTypeParser::GetCommandCode("D99").c_str());
	STRCMP_EQUAL("Dnn", DataTypeParser::GetCommandCode("D2147483647").c_str());
}

TEST(GetCommandCode, PlotState) {
	STRCMP_EQUAL("G75", DataTypeParser::GetCommandCode("G75").c_str());
	STRCMP_EQUAL("G01", DataTypeParser::GetCommandCode("G01").c_str());
	STRCMP_EQUAL("G02", DataTypeParser::GetCommandCode("G02").c_str());
	STRCMP_EQUAL("G03", DataTypeParser::GetCommandCode("G03").c_str());
}

TEST(GetCommandCode, Operations) {
	std::vector<std::string> opcodes = { "D01", "D02", "D03" };
	for (std::string &op : opcodes) {
		std::vector<std::string> words = { "X250000Y155000", "Y155000",
				"X250000", "", "X75000Y50000I40000J0", "I40000J0" };
		for (std::string &w : words) {
			STRCMP_EQUAL(op.c_str(), DataTypeParser::GetCommandCode(w + op).c_str());
		}
	}
}

TEST(GetCommandCode, ApertureTransformations) {
	STRCMP_EQUAL("LP", DataTypeParser::GetCommandCode("LPC").c_str());
	STRCMP_EQUAL("LM", DataTypeParser::GetCommandCode("LMN").c_str());
	STRCMP_EQUAL("LR", DataTypeParser::GetCommandCode("LR90").c_str());
	STRCMP_EQUAL("LS", DataTypeParser::GetCommandCode("LS0.8").c_str());
}

TEST(GetCommandCode, RegionStatement) {
	STRCMP_EQUAL("G36", DataTypeParser::GetCommandCode("G36").c_str());
	STRCMP_EQUAL("G37", DataTypeParser::GetCommandCode("G37").c_str());
}

TEST(GetCommandCode, BlockAperture) {
	STRCMP_EQUAL("AB", DataTypeParser::GetCommandCode("ABD12").c_str());
	STRCMP_EQUAL("AB", DataTypeParser::GetCommandCode("AB").c_str());
}

TEST(GetCommandCode, StepAndRepeat) {
	STRCMP_EQUAL("SR", DataTypeParser::GetCommandCode("SRX2Y3I2.0J3.0").c_str());
	STRCMP_EQUAL("SR", DataTypeParser::GetCommandCode("SR").c_str());
}

TEST(GetCommandCode, EndOfFile) {
	STRCMP_EQUAL("M02", DataTypeParser::GetCommandCode("M02").c_str());
}

TEST(GetCommandCode, Attributes) {
	STRCMP_EQUAL("TF", DataTypeParser::GetCommandCode("TFMyAttribute,Yes").c_str());
	STRCMP_EQUAL("TA", DataTypeParser::GetCommandCode("TA.AperFunction,ComponentPad").c_str());
	STRCMP_EQUAL("TO", DataTypeParser::GetCommandCode("TO.C,R6").c_str());
	STRCMP_EQUAL("TD", DataTypeParser::GetCommandCode("TD.AperFunction").c_str());
}

TEST(GetCommandCode, Unknown) {
	CHECK_THROWS(std::invalid_argument, DataTypeParser::GetCommandCode("Z"));
}

/**
 * Split Params
 */


TEST_GROUP(SplitParams) {
};

TEST(SplitParams, Empty) {
	std::string field = "";
	Parameters params = DataTypeParser::SplitParams(field, 'X');
	CHECK(params.empty());
}

TEST(SplitParams, JustDelim) {
	std::string field = "X";
	CHECK_THROWS(std::invalid_argument, DataTypeParser::SplitParams(field, 'X'));
}

TEST(SplitParams, NonNumeric) {
	std::string field = "0.01XABC";
	CHECK_THROWS(std::invalid_argument, DataTypeParser::SplitParams(field, 'X'));
}

TEST(SplitParams, OneParam) {
	std::string field = "123";
	Parameters params = DataTypeParser::SplitParams(field, 'X');

	LONGS_EQUAL(1, params.size());
	DOUBLES_EQUAL(123.0, *params.begin(), DBL_TOL);
}

TEST(SplitParams, ThreeParams) {
	std::string field = "0.030X0.040X0.0015";
	Parameters params = DataTypeParser::SplitParams(field, 'X');

	LONGS_EQUAL(3, params.size());
	DOUBLES_EQUAL(0.03, params[0], DBL_TOL);
	DOUBLES_EQUAL(0.04, params[1], DBL_TOL);
	DOUBLES_EQUAL(0.0015, params[2], DBL_TOL);
}

/**
 * Patterns
 */


TEST_GROUP(ParserPatterns) {
};

TEST(ParserPatterns, Name) {
	std::regex regex(DataTypeParser::GetNamePattern());
	LONGS_EQUAL_TEXT(0, regex.mark_count(), "pattern must not have subexpressions");
}

TEST(ParserPatterns, Field) {
	std::regex regex(DataTypeParser::GetFieldPattern());
	LONGS_EQUAL_TEXT(0, regex.mark_count(), "pattern must not have subexpressions");
}

TEST(ParserPatterns, Number) {
	std::regex regex(DataTypeParser::GetNumberPattern());
	LONGS_EQUAL_TEXT(0, regex.mark_count(), "pattern must not have subexpressions");
}
