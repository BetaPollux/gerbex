/*
 * test_SyntaxParser.cpp
 *
 *  Created on: Mar. 24, 2024
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

#include "SyntaxParser.h"
#include <memory>
#include <sstream>
#include "CppUTest/TestHarness.h"

void setIstream(SyntaxParser &parser, std::string const &str) {
	parser.SetIstream(std::make_unique<std::istringstream>(str));
}

TEST_GROUP(SyntaxParserTest) {
	SyntaxParser parser;
};

TEST(SyntaxParserTest, Word) {
	setIstream(parser, "D10*");

	std::vector<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words[0].c_str());
}

TEST(SyntaxParserTest, Word_LeadingWhitespace) {
	setIstream(parser, "\n\nD10*");

	std::vector<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words[0].c_str());
}

TEST(SyntaxParserTest, Word_LeadingWhitespace_Dos) {
	setIstream(parser, "\r\n\r\nD10*");

	std::vector<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words[0].c_str());
}

TEST(SyntaxParserTest, Two_Word) {
	setIstream(parser, "D10*X0Y0D02*");

	std::vector<std::string> first = parser.GetNextCommand();
	std::vector<std::string> second = parser.GetNextCommand();

	CHECK(!first.empty());
	STRCMP_EQUAL("D10", first[0].c_str());
	CHECK(!second.empty());
	STRCMP_EQUAL("X0Y0D02", second[0].c_str());
}

TEST(SyntaxParserTest, Extended) {
	setIstream(parser, "%FSLAX26Y26*%");

	std::vector<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words[0].c_str());
}

TEST(SyntaxParserTest, Extended_LeadingWhitespace) {
	setIstream(parser, "\n\n%FSLAX26Y26*%");

	std::vector<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words[0].c_str());
}

TEST(SyntaxParserTest, Extended_LeadingWhitespace_Dos) {
	setIstream(parser, "\r\n\r\n%FSLAX26Y26*%");

	std::vector<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words[0].c_str());
}

TEST(SyntaxParserTest, Two_Extended) {
	setIstream(parser, "%FSLAX26Y26*%%MOMM*%");

	std::vector<std::string> first = parser.GetNextCommand();
	std::vector<std::string> second = parser.GetNextCommand();

	CHECK(!first.empty());
	STRCMP_EQUAL("FSLAX26Y26", first[0].c_str());
	CHECK(!second.empty());
	STRCMP_EQUAL("MOMM", second[0].c_str());
}

TEST(SyntaxParserTest, ExtendedMulti) {
	setIstream(parser, "%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%");

	std::vector<std::string> words = parser.GetNextCommand();

	LONGS_EQUAL(3, words.size());
	STRCMP_EQUAL("AMDONUTVAR", words[0].c_str());
	STRCMP_EQUAL("1,1,$1,$2,$3", words[1].c_str());
	STRCMP_EQUAL("1,0,$4,$2,$3", words[2].c_str());
}

TEST(SyntaxParserTest, ExtendedMulti_Multiline) {
	setIstream(parser,
			"%AMTriangle_30*\n4,1,3,\n1,-1,\n1,1,\n2,1,\n1,-1,\n30*\n%");

	std::vector<std::string> words = parser.GetNextCommand();

	LONGS_EQUAL(2, words.size());
	STRCMP_EQUAL("AMTriangle_30", words[0].c_str());
	STRCMP_EQUAL("4,1,3,1,-1,1,1,2,1,1,-1,30", words[1].c_str());
}

TEST(SyntaxParserTest, CurrentLine_One) {
	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(SyntaxParserTest, CurrentLine_ReadOne) {
	setIstream(parser, "%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%");

	parser.GetNextCommand();

	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(SyntaxParserTest, CurrentLine_ReadTwo) {
	setIstream(parser, "D10*\nX0Y0D02*");

	parser.GetNextCommand();
	parser.GetNextCommand();

	LONGS_EQUAL(2, parser.GetCurrentLine());
}

TEST(SyntaxParserTest, SetStreamResetsCount) {
	setIstream(parser, "\n\n\nD10*");

	parser.GetNextCommand();
	parser.GetNextCommand();

	setIstream(parser, "");
	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(SyntaxParserTest, CurrentLine_Many) {
	setIstream(parser,
			"%AMTriangle_30*\n4,1,3,\n1,-1,\n1,1,\n2,1,\n1,-1,\n30*\n%");

	parser.GetNextCommand();

	LONGS_EQUAL(8, parser.GetCurrentLine());
}

TEST(SyntaxParserTest, Empty) {
	std::vector<std::string> words = parser.GetNextCommand();

	CHECK(words.empty());
}

TEST(SyntaxParserTest, Finish) {
	setIstream(parser, "D10*");

	std::vector<std::string> words = parser.GetNextCommand();
	std::vector<std::string> final = parser.GetNextCommand();

	CHECK(!words.empty());
	CHECK(final.empty());
}

TEST(SyntaxParserTest, NoDelimiter) {
	setIstream(parser, "WHAT");

	CHECK_THROWS(std::runtime_error, parser.GetNextCommand());
}

TEST(SyntaxParserTest, OpenExtended) {
	setIstream(parser, "%OHNO*");

	CHECK_THROWS(std::runtime_error, parser.GetNextCommand());
}

