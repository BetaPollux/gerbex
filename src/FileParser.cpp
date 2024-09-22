/*
 * FileParser.cpp
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
#include <sstream>

namespace gerbex {

const char EXT_DELIM = '%';
const char WORD_DELIM = '*';

FileParser::FileParser() :
		FileParser(std::make_unique<std::istringstream>("")) {
	// Empty
}

FileParser::FileParser(std::unique_ptr<std::istream> stream) {
	SetIstream(std::move(stream));
}

FileParser::~FileParser() {
	// Empty
}

int removeNewlines(std::string &str) {
	// Removes newlines in the string argument and return how many were removed
	std::string result;
	int count = 0;
	for (char &c : str) {
		if (c == '\n') {
			count++;
			continue;
		} else if (c == '\r') {
			continue;
		}
		result += c;
	}

	str = result;
	return count;
}

std::vector<std::string> split(std::string &str, char delim) {
	std::vector<std::string> result;
	std::string new_str;
	for (char &c : str) {
		if (c == delim) {
			result.push_back(new_str);
			new_str = "";
		} else {
			new_str += c;
		}
	}
	if (!new_str.empty()) {
		result.push_back(new_str);
	}
	return result;
}

std::vector<std::string> FileParser::GetNextCommand() {
	// Discard all leading space, and count new lines
	while (isspace(m_istream->peek())) {
		int space = m_istream->get();
		if (space == '\n') {
			m_currentLine++;
		}
	}

	if (m_istream->eof()) {
		// Return empty vector on EOF
		return std::vector<std::string>();
	}

	std::string command_str;
	char delim;
	if (m_istream->peek() == EXT_DELIM) {
		// Handle extended command
		m_istream->get();	// Discard the leading EXT_DELIM
		delim = EXT_DELIM;
	} else {
		// Handle word command
		delim = WORD_DELIM;
	}

	std::getline(*m_istream, command_str, delim);
	if (m_istream->eof()) {
		throw std::runtime_error("reached EOF without a delimiter");
	}
	m_currentLine += removeNewlines(command_str);

	return split(command_str, WORD_DELIM);
}

void FileParser::SetIstream(std::unique_ptr<std::istream> istream) {
	m_istream = std::move(istream);
	m_currentLine = 1;
}

int FileParser::GetCurrentLine() const {
	return m_currentLine;
}

} /* namespace gerbex */
