/*
 * test_DataTypeParser.cpp
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

#include <stdexcept>
#include <vector>

#include "DataTypeParser.h"
#include "CppUTest/TestHarness.h"

#define DBL_TOL 1e-5

TEST_GROUP(DataTypeParserTest) {
	DataTypeParser parser;
};

TEST(DataTypeParserTest, BadType) {
	CHECK_THROWS(std::invalid_argument, parser.Match("255", "bad_type"));
}

TEST(DataTypeParserTest, GetPattern) {
	STRCMP_EQUAL("[0-9]+", parser.GetPattern("unsigned_integer").c_str());
	CHECK_THROWS(std::invalid_argument, parser.GetPattern("bad_type"));
}

TEST(DataTypeParserTest, UnsignedInteger) {
	LONGS_EQUAL(0, parser.UnsignedInteger("0"));
	LONGS_EQUAL(16, parser.UnsignedInteger("16"));
	LONGS_EQUAL(255, parser.UnsignedInteger("000255"));
	CHECK_THROWS(std::invalid_argument, parser.UnsignedInteger("-1024"));
	CHECK_THROWS(std::invalid_argument, parser.UnsignedInteger("+16"));
	CHECK_THROWS(std::invalid_argument, parser.UnsignedInteger("0X"));
}

TEST(DataTypeParserTest, PositiveInteger) {
	LONGS_EQUAL(16, parser.PositiveInteger("16"));
	LONGS_EQUAL(255, parser.PositiveInteger("000255"));
	CHECK_THROWS(std::invalid_argument, parser.PositiveInteger("0"));
	CHECK_THROWS(std::invalid_argument, parser.PositiveInteger("-1024"));
	CHECK_THROWS(std::invalid_argument, parser.PositiveInteger("+16"));
	CHECK_THROWS(std::invalid_argument, parser.PositiveInteger("0X"));
}

TEST(DataTypeParserTest, Integer) {
	LONGS_EQUAL(0, parser.Integer("0"));
	LONGS_EQUAL(16, parser.Integer("16"));
	LONGS_EQUAL(-1024, parser.Integer("-1024"));
	LONGS_EQUAL(16, parser.Integer("+16"));
	LONGS_EQUAL(255, parser.Integer("+000255"));
	CHECK_THROWS(std::invalid_argument, parser.Integer("0X"));
}

TEST(DataTypeParserTest, Decimal) {
	DOUBLES_EQUAL(0.0, parser.Decimal("0"), DBL_TOL);
	DOUBLES_EQUAL(-200.0, parser.Decimal("-200"), DBL_TOL);
	DOUBLES_EQUAL(5000.0, parser.Decimal("+5000"), DBL_TOL);
	DOUBLES_EQUAL(1234.56, parser.Decimal("001234.56"), DBL_TOL);
	DOUBLES_EQUAL(0.123, parser.Decimal(".123"), DBL_TOL);
	DOUBLES_EQUAL(-0.128, parser.Decimal("-0.128"), DBL_TOL);
	CHECK_THROWS(std::invalid_argument, parser.Decimal("0X"));
}

TEST(DataTypeParserTest, UnsignedDecimal) {
	DOUBLES_EQUAL(0.0, parser.UnsignedDecimal("0"), DBL_TOL);
	DOUBLES_EQUAL(5000.0, parser.UnsignedDecimal("5000"), DBL_TOL);
	DOUBLES_EQUAL(1234.56, parser.UnsignedDecimal("001234.56"), DBL_TOL);
	DOUBLES_EQUAL(0.123, parser.UnsignedDecimal(".123"), DBL_TOL);
	CHECK_THROWS(std::invalid_argument, parser.UnsignedDecimal("+200"));
	CHECK_THROWS(std::invalid_argument, parser.UnsignedDecimal("-200"));
	CHECK_THROWS(std::invalid_argument, parser.UnsignedDecimal("-0.128"));
	CHECK_THROWS(std::invalid_argument, parser.UnsignedDecimal("0X"));
}

TEST(DataTypeParserTest, String) {
	STRCMP_EQUAL("", parser.String("").c_str());
	STRCMP_EQUAL("123,ABC", parser.String("123,ABC").c_str());
	CHECK_THROWS(std::invalid_argument, parser.String("%ABC"));
	CHECK_THROWS(std::invalid_argument, parser.String("ABC*"));
}

TEST(DataTypeParserTest, Field) {
	STRCMP_EQUAL("", parser.Field("").c_str());
	STRCMP_EQUAL("123ABC", parser.Field("123ABC").c_str());
	CHECK_THROWS(std::invalid_argument, parser.Field("ABC,123"));
	CHECK_THROWS(std::invalid_argument, parser.Field("%ABC"));
	CHECK_THROWS(std::invalid_argument, parser.Field("ABC*"));
}

TEST(DataTypeParserTest, Name) {
	STRCMP_EQUAL(".ABC", parser.Name(".ABC").c_str());
	STRCMP_EQUAL("$3", parser.Name("$3").c_str());
	STRCMP_EQUAL(".3abc", parser.Name(".3abc").c_str());
	STRCMP_EQUAL("ABC.123", parser.Name("ABC.123").c_str());
	CHECK_THROWS(std::invalid_argument, parser.Name(""));
	CHECK_THROWS(std::invalid_argument, parser.Name("3"));
	CHECK_THROWS(std::invalid_argument, parser.Name("%ABC"));
	CHECK_THROWS(std::invalid_argument, parser.Name("ABC*"));
}
