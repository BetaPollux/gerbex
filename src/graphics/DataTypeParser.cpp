/*
 * DataTypeParser.cpp
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

#include "DataTypeParser.h"
#include <regex>
#include <stdexcept>

namespace gerbex {

const std::string DataTypeParser::GetNumberPattern() {
	return "[+-]?[0-9]*[\\.]?[0-9]+";
}

const std::string DataTypeParser::GetNamePattern() {
	return "[._$a-zA-Z][._$a-zA-Z0-9]{0,126}";
}

const std::string DataTypeParser::GetFieldPattern() {
	return "[^%*,]*";
}

std::string DataTypeParser::Match(const std::string &word,
		const std::string &pattern) {
	std::smatch match;
	std::regex regex(pattern);
	if (std::regex_match(word, match, regex)) {
		return match[0].str();
	}
	throw std::invalid_argument("invalid string");
}

std::string DataTypeParser::GetCommandCode(const std::string &word) {
	std::smatch match;
	if (std::regex_search(word, match,
			std::regex("^([A-Z]{2}|[GM][0-9]{2})"))) {
		return match[0].str();
	} else if (std::regex_search(word, match, std::regex("D([0-9]+)$"))) {
		int ident = std::stoi(match[1].str());
		if (ident < 10) {
			return match[0].str();	// D0n
		} else {
			return "Dnn";
		}
	}
	throw std::invalid_argument("unrecognized word");
}

Parameters DataTypeParser::SplitParams(const std::string &field, char delim) {
	Parameters params;
	if (field.empty()) {
		return params;
	}

	std::smatch match;
	std::regex regex(GetNumberPattern());
	std::istringstream istr(field);
	while (!istr.eof()) {
		std::string param_str;
		std::getline(istr, param_str, delim);

		if (std::regex_match(param_str, match, regex)) {
			// Regex is needed because std::stod stops consuming at non-numeric char
			params.push_back(std::stod(param_str));
		} else {
			throw std::invalid_argument(
					"invalid non-numeric parameter: " + param_str);
		}
	}

	return params;
}

} /* namespace gerbex */

