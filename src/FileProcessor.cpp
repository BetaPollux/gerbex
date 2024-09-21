/*
 * FileProcessor.cpp
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

#include <iostream>
#include "CommandHandler.h"
#include "DataTypeParser.h"
#include "FileParser.h"
#include "FileProcessor.h"


FileProcessor::FileProcessor() {
	m_handlers = {
		{"G04", CommandHandler::Comment},
		{"MO", CommandHandler::Unit},
		{"FS", CommandHandler::Format},
		{"AD", CommandHandler::ApertureDefine},
		{"AM", CommandHandler::ApertureMacro},
		{"Dnn", CommandHandler::SetCurrentAperture},
		{"G75", CommandHandler::NotImplemented},
		{"G01", CommandHandler::PlotState},
		{"G02", CommandHandler::PlotState},
		{"G03", CommandHandler::PlotState},
		{"D01", CommandHandler::Plot},
		{"D02", CommandHandler::Move},
		{"D03", CommandHandler::Flash},
		{"LP", CommandHandler::ApertureTransformations},
		{"LM", CommandHandler::ApertureTransformations},
		{"LR", CommandHandler::ApertureTransformations},
		{"LS", CommandHandler::ApertureTransformations},
		{"G36", CommandHandler::RegionStatement},
		{"G37", CommandHandler::RegionStatement},
		{"AB", CommandHandler::BlockAperture},
		{"SR", CommandHandler::StepAndRepeat},
		{"M02", CommandHandler::EndOfFile},
		{"TF", CommandHandler::NotImplemented},
		{"TA", CommandHandler::NotImplemented},
		{"TO", CommandHandler::NotImplemented},
		{"TD", CommandHandler::NotImplemented}
	};
}

FileProcessor::~FileProcessor() {
	// Empty
}

void FileProcessor::Process(std::unique_ptr<std::istream> stream) {
	FileParser parser(std::move(stream));
	while (true) {
		std::vector<std::string> words = parser.GetNextCommand();

		if (words.empty()) {
			break;	// EOF
		}
		try {
			std::string code = DataTypeParser::GetCommandCode(words[0]);
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

CommandsProcessor& FileProcessor::GetProcessor() {
	return m_processor;
}

