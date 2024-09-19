/*
 * GerberFileProcessor.cpp
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
#include "GerberFileProcessor.h"
#include "SyntaxParser.h"

#include <iostream>

GerberFileProcessor::GerberFileProcessor() {
	// Empty
}

GerberFileProcessor::~GerberFileProcessor() {
	// Empty
}

void GerberFileProcessor::Process(std::unique_ptr<std::istream> stream) {
	SyntaxParser parser(std::move(stream));
	while (true) {
		std::vector<std::string> words = parser.GetNextCommand();

		if (words.size() == 0) {
			break;	// EOF
		}
		try {
			std::string code = CommandParser::GetCommandCode(words[0]);
			if (code == "FS") {
				m_processor.SetFormat(CoordinateFormat::FromCommand(words[0]));
			} else if (code == "MO") {
				m_processor.SetUnit(GraphicsState::UnitFromCommand(words[0]));
			} else if (code == "M02") {
				m_processor.SetEndOfFile();
			}
		} catch (const std::invalid_argument &ex) {
			std::cerr << "WARNING line " << parser.GetCurrentLine() << ": " << ex.what() << ": " << words[0] << std::endl;
			continue;
		}
	}
}

CommandsProcessor& GerberFileProcessor::GetProcessor() {
	return m_processor;
}
