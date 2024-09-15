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
#include <stdexcept>

std::string CommandParser::GetCommandCode(const std::string &word) {
	char c = word[0];
	if (c == 'G') {
		return word.substr(0, 3);
	} else if (c == 'D') {
		return "Dnn";
	} else if (c == 'X' || c == 'Y' || c == 'I' || c == 'J') {
		return word.substr(word.length() - 3, 3);
	} else if (c == 'L' || c == 'F') {
		return word.substr(0, 2);
	}

	std::string cc = word.substr(0, 2);
	if (cc == "SR") {
		if (word.length() == 2) {
			return "SR_close";
		} else {
			return "SR_open";
		}
	} else if (cc == "AB") {
		if (word.length() == 2) {
			return "AB_close";
		} else {
			return "AB_open";
		}
	} else if (cc == "AM" || cc == "AD" || cc == "MO") {
		return cc;
	}

	return word;
}
