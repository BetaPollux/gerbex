/*
 * test_FileParser.cpp
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

#include "FileParser.h"
#include <memory>
#include <sstream>
#include "CppUTest/TestHarness.h"

using namespace gerbex;



void setIstream(FileParser &parser, std::string const &str) {
	parser.SetIstream(std::make_unique<std::istringstream>(str));
}

/**
 * Get Next Command
 */

TEST_GROUP(FileParser_GetNext) {
	FileParser parser;
};

TEST(FileParser_GetNext, Word) {
	setIstream(parser, "D10*");

	std::list<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words.begin()->c_str());
}

TEST(FileParser_GetNext, Word_LeadingWhitespace) {
	setIstream(parser, "\n\nD10*");

	std::list<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words.begin()->c_str());
}

TEST(FileParser_GetNext, Word_LeadingWhitespace_Dos) {
	setIstream(parser, "\r\n\r\nD10*");

	std::list<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words.begin()->c_str());
}

TEST(FileParser_GetNext, Two_Word) {
	setIstream(parser, "D10*X0Y0D02*");

	std::list<std::string> first = parser.GetNextCommand();
	std::list<std::string> second = parser.GetNextCommand();

	CHECK(!first.empty());
	STRCMP_EQUAL("D10", first.begin()->c_str());
	CHECK(!second.empty());
	STRCMP_EQUAL("X0Y0D02", second.begin()->c_str());
}

TEST(FileParser_GetNext, Extended) {
	setIstream(parser, "%FSLAX26Y26*%");

	std::list<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words.begin()->c_str());
}

TEST(FileParser_GetNext, Extended_LeadingWhitespace) {
	setIstream(parser, "\n\n%FSLAX26Y26*%");

	std::list<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words.begin()->c_str());
}

TEST(FileParser_GetNext, Extended_LeadingWhitespace_Dos) {
	setIstream(parser, "\r\n\r\n%FSLAX26Y26*%");

	std::list<std::string> words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words.begin()->c_str());
}

TEST(FileParser_GetNext, Two_Extended) {
	setIstream(parser, "%FSLAX26Y26*%%MOMM*%");

	std::list<std::string> first = parser.GetNextCommand();
	std::list<std::string> second = parser.GetNextCommand();

	CHECK(!first.empty());
	STRCMP_EQUAL("FSLAX26Y26", first.begin()->c_str());
	CHECK(!second.empty());
	STRCMP_EQUAL("MOMM", second.begin()->c_str());
}

TEST(FileParser_GetNext, ExtendedMulti) {
	setIstream(parser, "%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%");

	std::list<std::string> words = parser.GetNextCommand();

	LONGS_EQUAL(3, words.size());
	auto it = words.begin();
	STRCMP_EQUAL("AMDONUTVAR", it->c_str());
	STRCMP_EQUAL("1,1,$1,$2,$3", (++it)->c_str());
	STRCMP_EQUAL("1,0,$4,$2,$3", (++it)->c_str());
}

TEST(FileParser_GetNext, ExtendedMulti_Multiline) {
	setIstream(parser,
			"%AMTriangle_30*\n4,1,3,\n1,-1,\n1,1,\n2,1,\n1,-1,\n30*\n%");

	std::list<std::string> words = parser.GetNextCommand();

	auto it = words.begin();
	LONGS_EQUAL(2, words.size());
	STRCMP_EQUAL("AMTriangle_30", it->c_str());
	STRCMP_EQUAL("4,1,3,1,-1,1,1,2,1,1,-1,30", (++it)->c_str());
}

TEST(FileParser_GetNext, CurrentLine_One) {
	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, CurrentLine_ReadOne) {
	setIstream(parser, "%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%");

	parser.GetNextCommand();

	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, CurrentLine_ReadTwo) {
	setIstream(parser, "D10*\nX0Y0D02*");

	parser.GetNextCommand();
	parser.GetNextCommand();

	LONGS_EQUAL(2, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, SetStreamResetsCount) {
	setIstream(parser, "\n\n\nD10*");

	parser.GetNextCommand();
	parser.GetNextCommand();

	setIstream(parser, "");
	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, CurrentLine_Many) {
	setIstream(parser,
			"%AMTriangle_30*\n4,1,3,\n1,-1,\n1,1,\n2,1,\n1,-1,\n30*\n%");

	parser.GetNextCommand();

	LONGS_EQUAL(8, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, Empty) {
	std::list<std::string> words = parser.GetNextCommand();

	CHECK(words.empty());
}

TEST(FileParser_GetNext, Finish) {
	setIstream(parser, "D10*");

	std::list<std::string> words = parser.GetNextCommand();
	std::list<std::string> final = parser.GetNextCommand();

	CHECK(!words.empty());
	CHECK(final.empty());
}

TEST(FileParser_GetNext, NoDelimiter) {
	setIstream(parser, "WHAT");

	CHECK_THROWS(std::runtime_error, parser.GetNextCommand());
}

TEST(FileParser_GetNext, OpenExtended) {
	setIstream(parser, "%OHNO*");

	CHECK_THROWS(std::runtime_error, parser.GetNextCommand());
}

