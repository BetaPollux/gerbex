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
	m_handlers["G04"] = CommandParser::HandleComment;
	m_handlers["MO"] = CommandParser::HandleUnit;
	m_handlers["FS"] = CommandParser::HandleFormat;
	m_handlers["AD"] = CommandParser::HandleApertureDefine;
	m_handlers["AM"] = CommandParser::HandleApertureMacro;
	m_handlers["Dnn"] = CommandParser::HandleSetCurrentAperture;
	m_handlers["G75"] = CommandParser::NotImplemented;
	m_handlers["G01"] = CommandParser::HandlePlotState;
	m_handlers["G02"] = CommandParser::HandlePlotState;
	m_handlers["G03"] = CommandParser::HandlePlotState;
	m_handlers["D01"] = CommandParser::HandlePlot;
	m_handlers["D02"] = CommandParser::HandleMove;
	m_handlers["D03"] = CommandParser::HandleFlash;
	m_handlers["LP"] = CommandParser::HandleApertureTransformations;
	m_handlers["LM"] = CommandParser::HandleApertureTransformations;
	m_handlers["LR"] = CommandParser::HandleApertureTransformations;
	m_handlers["LS"] = CommandParser::HandleApertureTransformations;
	m_handlers["G36"] = CommandParser::HandleRegionStatement;
	m_handlers["G37"] = CommandParser::HandleRegionStatement;
	m_handlers["AB"] = CommandParser::HandleBlockAperture;
	m_handlers["SR"] = CommandParser::HandleStepAndRepeat;
	m_handlers["M02"] = CommandParser::HandleEndOfFile;
	m_handlers["TF"] = CommandParser::NotImplemented;
	m_handlers["TA"] = CommandParser::NotImplemented;
	m_handlers["TO"] = CommandParser::NotImplemented;
	m_handlers["TD"] = CommandParser::NotImplemented;
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
			auto handler = m_handlers.find(code);
			if (handler != m_handlers.end()) {
				handler->second(m_processor, words);
			} else {
				throw std::invalid_argument("unsupported command " + code);
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

