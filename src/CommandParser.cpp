/*
 * CommandParser.cpp
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
#include <regex>
#include <stdexcept>

std::string CommandParser::GetCommandCode(const std::string &word) {
	std::smatch match;
	if (std::regex_search(word, match, std::regex("^[XYIJ0-9]*(D\\d{2})$"))) {
		if (match[1].str()[1] == '0') {
			return match[1].str();	// D0n
		} else {
			return "Dnn";
		}
	} else if (std::regex_search(word, match, std::regex("^(AB|SR)(\\S+)?"))) {
		if (match[2].matched) {
			return match[1].str() + "_open";
		} else {
			return match[1].str() + "_close";
		}
	} else if (std::regex_search(word, match, std::regex("^A[DM]"))) {
		return match[0].str();
	} else if (std::regex_search(word, match, std::regex("^[GM]\\d{2}"))) {
		return match[0].str();
	} else if (std::regex_search(word, match, std::regex("^(L\\w|MO|FS)"))) {
		return match[0].str();
	}
	throw std::invalid_argument("unrecognized word");
}
