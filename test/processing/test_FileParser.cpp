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

/**
 * Get Next Command
 */

TEST_GROUP(FileParser_GetNext) {
};

TEST(FileParser_GetNext, Word) {
	std::istringstream stream("D10*");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words.begin()->c_str());
}

TEST(FileParser_GetNext, Word_LeadingWhitespace) {
	std::istringstream stream("\n\nD10*");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words.begin()->c_str());
}

TEST(FileParser_GetNext, Word_LeadingWhitespace_Dos) {
	std::istringstream stream("\r\n\r\nD10*");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("D10", words.begin()->c_str());
}

TEST(FileParser_GetNext, Two_Word) {
	std::istringstream stream("D10*X0Y0D02*");
	FileParser parser(stream);

	Fields first = parser.GetNextCommand();
	Fields second = parser.GetNextCommand();

	CHECK(!first.empty());
	STRCMP_EQUAL("D10", first.begin()->c_str());
	CHECK(!second.empty());
	STRCMP_EQUAL("X0Y0D02", second.begin()->c_str());
}

TEST(FileParser_GetNext, Extended) {
	std::istringstream stream("%FSLAX26Y26*%");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words.begin()->c_str());
}

TEST(FileParser_GetNext, Extended_LeadingWhitespace) {
	std::istringstream stream("\n\n%FSLAX26Y26*%");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words.begin()->c_str());
}

TEST(FileParser_GetNext, Extended_LeadingWhitespace_Dos) {
	std::istringstream stream("\r\n\r\n%FSLAX26Y26*%");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	CHECK(!words.empty());
	STRCMP_EQUAL("FSLAX26Y26", words.begin()->c_str());
}

TEST(FileParser_GetNext, Two_Extended) {
	std::istringstream stream("%FSLAX26Y26*%%MOMM*%");
	FileParser parser(stream);

	Fields first = parser.GetNextCommand();
	Fields second = parser.GetNextCommand();

	CHECK(!first.empty());
	STRCMP_EQUAL("FSLAX26Y26", first.begin()->c_str());
	CHECK(!second.empty());
	STRCMP_EQUAL("MOMM", second.begin()->c_str());
}

TEST(FileParser_GetNext, ExtendedMulti) {
	std::istringstream stream("%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	LONGS_EQUAL(3, words.size());
	STRCMP_EQUAL("AMDONUTVAR", words[0].c_str());
	STRCMP_EQUAL("1,1,$1,$2,$3", words[1].c_str());
	STRCMP_EQUAL("1,0,$4,$2,$3", words[2].c_str());
}

TEST(FileParser_GetNext, ExtendedMulti_Multiline) {
	std::istringstream stream("%AMTriangle_30*\n4,1,3,\n1,-1,\n1,1,\n2,1,\n1,-1,\n30*\n%");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();

	LONGS_EQUAL(2, words.size());
	STRCMP_EQUAL("AMTriangle_30", words[0].c_str());
	STRCMP_EQUAL("4,1,3,1,-1,1,1,2,1,1,-1,30", words[1].c_str());
}

TEST(FileParser_GetNext, CurrentLine_One) {
	std::istringstream stream("D10*");
	FileParser parser(stream);
	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, CurrentLine_ReadOne) {
	std::istringstream stream("%AMDONUTVAR*1,1,$1,$2,$3*1,0,$4,$2,$3*%");
	FileParser parser(stream);

	parser.GetNextCommand();

	LONGS_EQUAL(1, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, CurrentLine_ReadTwo) {
	std::istringstream stream("D10*\nX0Y0D02*");
	FileParser parser(stream);

	parser.GetNextCommand();
	parser.GetNextCommand();

	LONGS_EQUAL(2, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, CurrentLine_Many) {
	std::istringstream stream("%AMTriangle_30*\n4,1,3,\n1,-1,\n1,1,\n2,1,\n1,-1,\n30*\n%");
	FileParser parser(stream);

	parser.GetNextCommand();

	LONGS_EQUAL(8, parser.GetCurrentLine());
}

TEST(FileParser_GetNext, Empty) {
	std::istringstream stream("");
	FileParser parser(stream);
	Fields words = parser.GetNextCommand();

	CHECK(words.empty());
}

TEST(FileParser_GetNext, Finish) {
	std::istringstream stream("D10*");
	FileParser parser(stream);

	Fields words = parser.GetNextCommand();
	Fields final = parser.GetNextCommand();

	CHECK(!words.empty());
	CHECK(final.empty());
}

TEST(FileParser_GetNext, NoDelimiter) {
std::istringstream stream("WHAT");
	FileParser parser(stream);

	CHECK_THROWS(std::runtime_error, parser.GetNextCommand());
}

TEST(FileParser_GetNext, OpenExtended) {
	std::istringstream stream("%OHNO*");
	FileParser parser(stream);

	CHECK_THROWS(std::runtime_error, parser.GetNextCommand());
}
