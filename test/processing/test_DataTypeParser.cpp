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

TEST(DataTypeParserTest, UnsignedInteger) {
	LONGS_EQUAL(0, DataTypeParser::UnsignedInteger("0"));
	LONGS_EQUAL(16, DataTypeParser::UnsignedInteger("16"));
	LONGS_EQUAL(255, DataTypeParser::UnsignedInteger("000255"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::UnsignedInteger("-1024"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::UnsignedInteger("+16"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::UnsignedInteger("0X"));
}

TEST(DataTypeParserTest, PositiveInteger) {
	LONGS_EQUAL(16, DataTypeParser::PositiveInteger("16"));
	LONGS_EQUAL(255, DataTypeParser::PositiveInteger("000255"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::PositiveInteger("0"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::PositiveInteger("-1024"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::PositiveInteger("+16"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::PositiveInteger("0X"));
}

TEST(DataTypeParserTest, Integer) {
	LONGS_EQUAL(0, DataTypeParser::Integer("0"));
	LONGS_EQUAL(16, DataTypeParser::Integer("16"));
	LONGS_EQUAL(-1024, DataTypeParser::Integer("-1024"));
	LONGS_EQUAL(16, DataTypeParser::Integer("+16"));
	LONGS_EQUAL(255, DataTypeParser::Integer("+000255"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Integer("0X"));
}

TEST(DataTypeParserTest, Decimal) {
	DOUBLES_EQUAL(0.0, DataTypeParser::Decimal("0"), DBL_TOL);
	DOUBLES_EQUAL(-200.0, DataTypeParser::Decimal("-200"), DBL_TOL);
	DOUBLES_EQUAL(5000.0, DataTypeParser::Decimal("+5000"), DBL_TOL);
	DOUBLES_EQUAL(1234.56, DataTypeParser::Decimal("001234.56"), DBL_TOL);
	DOUBLES_EQUAL(0.123, DataTypeParser::Decimal(".123"), DBL_TOL);
	DOUBLES_EQUAL(-0.128, DataTypeParser::Decimal("-0.128"), DBL_TOL);
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Decimal("0X"));
}

TEST(DataTypeParserTest, UnsignedDecimal) {
	DOUBLES_EQUAL(0.0, DataTypeParser::UnsignedDecimal("0"), DBL_TOL);
	DOUBLES_EQUAL(5000.0, DataTypeParser::UnsignedDecimal("5000"), DBL_TOL);
	DOUBLES_EQUAL(1234.56, DataTypeParser::UnsignedDecimal("001234.56"), DBL_TOL);
	DOUBLES_EQUAL(0.123, DataTypeParser::UnsignedDecimal(".123"), DBL_TOL);
	CHECK_THROWS(std::invalid_argument, DataTypeParser::UnsignedDecimal("+200"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::UnsignedDecimal("-200"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::UnsignedDecimal("-0.128"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::UnsignedDecimal("0X"));
}

TEST(DataTypeParserTest, Field) {
	STRCMP_EQUAL("", DataTypeParser::Field("").c_str());
	STRCMP_EQUAL("123ABC", DataTypeParser::Field("123ABC").c_str());
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Field("ABC,123"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Field("%ABC"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Field("ABC*"));
}

TEST(DataTypeParserTest, Name) {
	STRCMP_EQUAL(".ABC", DataTypeParser::Name(".ABC").c_str());
	STRCMP_EQUAL("$3", DataTypeParser::Name("$3").c_str());
	STRCMP_EQUAL(".3abc", DataTypeParser::Name(".3abc").c_str());
	STRCMP_EQUAL("ABC.123", DataTypeParser::Name("ABC.123").c_str());
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Name(""));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Name("3"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Name("%ABC"));
	CHECK_THROWS(std::invalid_argument, DataTypeParser::Name("ABC*"));
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
 * Split Fields
 */


TEST_GROUP(SplitFields) {
};

TEST(SplitFields, Empty) {
	std::string word = "";
	std::vector<std::string> fields = DataTypeParser::SplitFields(word);

	LONGS_EQUAL(1, fields.size());
	STRCMP_EQUAL("", fields[0].c_str());
}

TEST(SplitFields, OneField) {
	std::string word = "123";
	std::vector<std::string> fields = DataTypeParser::SplitFields(word);

	LONGS_EQUAL(1, fields.size());
	STRCMP_EQUAL("123", fields[0].c_str());
}

TEST(SplitFields, ThreeFields) {
	std::string word = "4,123,76";
	std::vector<std::string> fields = DataTypeParser::SplitFields(word);

	LONGS_EQUAL(3, fields.size());
	STRCMP_EQUAL("4", fields[0].c_str());
	STRCMP_EQUAL("123", fields[1].c_str());
	STRCMP_EQUAL("76", fields[2].c_str());
}

TEST(SplitFields, ThreeEmptyFields) {
	std::string word = ",,";
	std::vector<std::string> fields = DataTypeParser::SplitFields(word);

	LONGS_EQUAL(3, fields.size());
	STRCMP_EQUAL("", fields[0].c_str());
	STRCMP_EQUAL("", fields[1].c_str());
	STRCMP_EQUAL("", fields[2].c_str());
}

/**
 * Split Params
 */


TEST_GROUP(SplitParams) {
};

TEST(SplitParams, Empty) {
	std::string field = "X";
	CHECK_THROWS(std::invalid_argument, DataTypeParser::SplitParams(field));
}

TEST(SplitParams, NonNumeric) {
	std::string field = "0.01XABC";
	CHECK_THROWS(std::invalid_argument, DataTypeParser::SplitParams(field));
}

TEST(SplitParams, OneParam) {
	std::string field = "123";
	std::vector<double> params = DataTypeParser::SplitParams(field);

	LONGS_EQUAL(1, params.size());
	DOUBLES_EQUAL(123.0, params[0], DBL_TOL);
}

TEST(SplitParams, ThreeParams) {
	std::string field = "0.030X0.040X0.0015";
	std::vector<double> params = DataTypeParser::SplitParams(field);

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
