/*
 * SyntaxParser.cpp
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
#include <iterator>
#include <regex>

const char EXT_DELIM = '%';
const char WORD_DELIM = '*';

SyntaxParser::SyntaxParser() :
		m_istream { std::make_unique<std::istringstream>("") } {
	// Empty
}

SyntaxParser::SyntaxParser(std::unique_ptr<std::istream> stream) :
		m_istream { std::move(stream) } {
	// Empty
}

SyntaxParser::~SyntaxParser() {
	// Empty
}

std::string removeWhitespace(std::string &str) {
	// Whitespace in Gerber is just newlines
	return std::regex_replace(str, std::regex("[\r\n]+"), "");
}

std::vector<std::string> splitWords(std::string &str) {
	std::vector<std::string> result;
	std::regex words_regex("[^\\*]+");	//TODO build this using WORD_DELIM const
	auto words_begin = std::sregex_iterator(str.begin(), str.end(),
			words_regex);
	auto words_end = std::sregex_iterator();

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		result.push_back(i->str());
	}
	return result;
}

std::vector<std::string> SyntaxParser::GetNextCommand() {
	std::vector<std::string> command;
	while (isspace(m_istream->peek())) {
		m_istream->get();	// Discard all leading space
	}
	if (m_istream->peek() == EXT_DELIM) {
		// Handle extended command
		std::string extended;
		m_istream->get();	// Discard the leading EXT_DELIM
		std::getline(*m_istream, extended, EXT_DELIM);
		extended = removeWhitespace(extended);
		command = splitWords(extended);
	} else {
		// Handle word command
		std::string word;
		std::getline(*m_istream, word, WORD_DELIM);
		word = removeWhitespace(word);
		command.push_back(word);
	}
	return command;
}

void SyntaxParser::SetIstream(std::unique_ptr<std::istream> istream) {
	m_istream = std::move(istream);
}
