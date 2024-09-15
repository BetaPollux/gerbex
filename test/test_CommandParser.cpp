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

#include "CommandParser.h"
#include "CppUTest/TestHarness.h"

TEST_GROUP(CommandParserTest) {
};

/*
 * TODO: TF, TA, TO, TD
 *
 */

TEST(CommandParserTest, G04) {
	std::string code = CommandParser::GetCommandCode("G04 Comment");

	STRCMP_EQUAL("G04", code.c_str());
}

TEST(CommandParserTest, MO) {
	std::string code = CommandParser::GetCommandCode("MOMM");

	STRCMP_EQUAL("MO", code.c_str());
}

TEST(CommandParserTest, FS) {
	std::string code = CommandParser::GetCommandCode("FSLAX26Y26");

	STRCMP_EQUAL("FS", code.c_str());
}

TEST(CommandParserTest, AD) {
	std::string code = CommandParser::GetCommandCode("ADD10C,0.010");

	STRCMP_EQUAL("AD", code.c_str());
}

TEST(CommandParserTest, AM) {
	std::string code = CommandParser::GetCommandCode("AMTHERMAL80");
	STRCMP_EQUAL("AM", code.c_str());
}

TEST(CommandParserTest, Dnn) {
	std::string code = CommandParser::GetCommandCode("D10");

	STRCMP_EQUAL("Dnn", code.c_str());
}

TEST(CommandParserTest, G75) {
	std::string code = CommandParser::GetCommandCode("G75");

	STRCMP_EQUAL("G75", code.c_str());
}

TEST(CommandParserTest, G01) {
	std::string code = CommandParser::GetCommandCode("G01");

	STRCMP_EQUAL("G01", code.c_str());
}

TEST(CommandParserTest, G02) {
	std::string code = CommandParser::GetCommandCode("G02");

	STRCMP_EQUAL("G02", code.c_str());
}

TEST(CommandParserTest, G03) {
	std::string code = CommandParser::GetCommandCode("G03");

	STRCMP_EQUAL("G03", code.c_str());
}

TEST(CommandParserTest, D01) {
	//TODO test variants X, Y, I, J
	std::string code = CommandParser::GetCommandCode("X5000000Y0D01");

	STRCMP_EQUAL("D01", code.c_str());
}

TEST(CommandParserTest, D02) {
	//TODO test variants X, Y, I, J
	std::string code = CommandParser::GetCommandCode("X0Y0D02");

	STRCMP_EQUAL("D02", code.c_str());
}

TEST(CommandParserTest, D03) {
	//TODO test variants X, Y, I, J
	std::string code = CommandParser::GetCommandCode("X10000000Y10000000D03");

	STRCMP_EQUAL("D03", code.c_str());
}

TEST(CommandParserTest, LP) {
	std::string code = CommandParser::GetCommandCode("LPC");

	STRCMP_EQUAL("LP", code.c_str());
}

TEST(CommandParserTest, LM) {
	std::string code = CommandParser::GetCommandCode("LMN");

	STRCMP_EQUAL("LM", code.c_str());
}

TEST(CommandParserTest, LR) {
	std::string code = CommandParser::GetCommandCode("LR90");

	STRCMP_EQUAL("LR", code.c_str());
}

TEST(CommandParserTest, LS) {
	std::string code = CommandParser::GetCommandCode("LS0.8");

	STRCMP_EQUAL("LS", code.c_str());
}

TEST(CommandParserTest, G36) {
	std::string code = CommandParser::GetCommandCode("G36");

	STRCMP_EQUAL("G36", code.c_str());
}

TEST(CommandParserTest, G37) {
	std::string code = CommandParser::GetCommandCode("G37");

	STRCMP_EQUAL("G37", code.c_str());
}

TEST(CommandParserTest, AB_open) {
	std::string code = CommandParser::GetCommandCode("ABD12");

	STRCMP_EQUAL("AB_open", code.c_str());
}

TEST(CommandParserTest, AB_close) {
	std::string code = CommandParser::GetCommandCode("AB");

	STRCMP_EQUAL("AB_close", code.c_str());
}

TEST(CommandParserTest, SR_open) {
	std::string code = CommandParser::GetCommandCode("SRX2Y3I2.0J3.0");

	STRCMP_EQUAL("SR_open", code.c_str());
}

TEST(CommandParserTest, SR_close) {
	std::string code = CommandParser::GetCommandCode("SR");

	STRCMP_EQUAL("SR_close", code.c_str());
}

TEST(CommandParserTest, M02) {
	std::string code = CommandParser::GetCommandCode("M02");

	STRCMP_EQUAL("M02", code.c_str());
}

