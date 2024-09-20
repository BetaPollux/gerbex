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
#include "CoordinateData.h"
#include "DataTypeParser.h"
#include <iostream>
#include <regex>
#include <stdexcept>

std::string CommandParser::GetCommandCode(const std::string &word) {
	std::smatch match;
	if (std::regex_search(word, match, std::regex("^(A[BDM]|MO|FS|L[PMRS]|SR|T[FAOD])"))) {
			return match[0].str();
	} else if (std::regex_search(word, match, std::regex("D([0-9]+)$"))) {
		int ident = std::stoi(match[1].str());
		if (ident < 10) {
			return match[0].str();	// D0n
		} else {
			return "Dnn";
		}
	} else if (std::regex_search(word, match, std::regex("^[GM][0-9]{2}"))) {
		return match[0].str();
	}
	throw std::invalid_argument("unrecognized word");
}

void CommandParser::AssertWordCommand(const std::vector<std::string> &words) {
	if (words.size() != 1) {
		throw std::invalid_argument("expected word command, got extended command");
	}
}

void CommandParser::AssertCode(const std::string &code, const std::string &expected) {
	if (code != expected) {
		throw std::runtime_error("expected " + code + ", got " + expected);
	}
}

void CommandParser::NotImplemented(CommandsProcessor &processor, const std::vector<std::string> &words) {
	(void)processor;
	(void)words;
	throw std::invalid_argument("command not implemented");
}

void CommandParser::HandleComment(CommandsProcessor &processor, const std::vector<std::string> &words) {
	(void)processor;
	AssertWordCommand(words);
	// Ignore comment
}

void CommandParser::HandleUnit(CommandsProcessor &processor, const std::vector<std::string> &words) {
	AssertWordCommand(words);
	processor.SetUnit(GraphicsState::UnitFromCommand(words[0]));
}

void CommandParser::HandleFormat(CommandsProcessor &processor, const std::vector<std::string> &words) {
	AssertWordCommand(words);
	processor.SetFormat(CoordinateFormat::FromCommand(words[0]));
}

void CommandParser::HandleApertureDefine(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	DataTypeParser parser;

	std::string ident_re = parser.GetPattern("unsigned_integer");
	std::string name_re = parser.GetPattern("name");
	std::string field_re = parser.GetPattern("field");
	std::ostringstream pattern;
	pattern << "ADD(" << ident_re << ")";
	pattern << "(" << name_re << ")";
	pattern << ",(" << field_re << ")?";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words[0], match, regex)) {
		int ident = std::stoi(match[1].str());
		std::string name = match[2].str();
		std::vector<double> params = { 1.0 }; //TODO parse params
		std::shared_ptr<ApertureTemplate> aperture = processor.GetTemplate(name);
		processor.ApertureDefine(ident, aperture->Call(1, params.data()));
	} else {
		throw std::invalid_argument("invalid aperture define");
	}
}

void CommandParser::HandleApertureMacro(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	(void)processor;
	(void)words;
	throw std::invalid_argument("command not implemented");
}

void CommandParser::HandleSetCurrentAperture(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	DataTypeParser parser;
	std::smatch match;
	std::regex regex("D(" + parser.GetPattern("unsigned_integer") + ")");
	if (std::regex_search(words[0], match, regex)) {
		int ident = std::stoi(match[1].str());
		processor.SetCurrentAperture(ident);
	}
}

void CommandParser::HandlePlotState(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);

	PlotState state = GraphicsState::PlotStateFromCommand(words[0]);
	processor.SetPlotState(state);
}

void CommandParser::HandlePlot(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	AssertCode(words[0].substr(words[0].size() - 3, 3), "D01");

	CoordinateData coorddata = CoordinateData::FromString(words[0]);
	std::optional<Point> coord = coorddata.GetXY(processor.GetGraphicsState().GetCurrentPoint());
	std::optional<Point> offset = coorddata.GetIJ();

	if (!coord.has_value()) {
		throw std::logic_error("current point was not defined");
	}

	switch (processor.GetGraphicsState().GetPlotState()) {
		case PlotState::Linear:
			processor.PlotDraw(*coord);
			if (coorddata.GetIJ().has_value()) {
				throw std::invalid_argument("ignoring offset coordinate for linear Draw");
			}
			break;
		case PlotState::Clockwise:
		case PlotState::CounterClockwise:
			if (!offset.has_value()) {
				throw std::logic_error("circular plotting requires an offset coordinate");
			}
			processor.PlotArc(*coord, *offset);
			break;
		case PlotState::Undefined:
			throw std::logic_error("plot state was not defined");
	}
}

void CommandParser::HandleMove(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	AssertCode(words[0].substr(words[0].size() - 3, 3), "D02");

	CoordinateData coorddata = CoordinateData::FromString(words[0]);
	std::optional<Point> coord = coorddata.GetXY(processor.GetGraphicsState().GetCurrentPoint());

	if (!coord.has_value()) {
		throw std::logic_error("current point was not defined");
	}
	processor.Move(*coord);
	if (coorddata.GetIJ().has_value()) {
		throw std::invalid_argument("ignoring offset coordinate for Move");
	}
}

void CommandParser::HandleFlash(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	AssertCode(words[0].substr(words[0].size() - 3, 3), "D03");

	CoordinateData coorddata = CoordinateData::FromString(words[0]);
	std::optional<Point> coord = coorddata.GetXY(processor.GetGraphicsState().GetCurrentPoint());

	if (!coord.has_value()) {
		throw std::logic_error("current point was not defined");
	}
	processor.Flash(*coord);
	if (coorddata.GetIJ().has_value()) {
		throw std::invalid_argument("ignoring offset coordinate for Flash");
	}
}

void CommandParser::HandleApertureTransformations(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	DataTypeParser parser;

	std::string decimal_re = parser.GetPattern("decimal");
	std::ostringstream pattern;
	pattern << "L([PMRS])";
	pattern << "([CDNXY]+|" << decimal_re << ")";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words[0], match, regex)) {
		std::string param = match[1].str();
		std::string option = match[2].str();
		if (param == "P") {
			Polarity polarity = ApertureTransformation::PolarityFromCommand(option);
			processor.GetTransformation().SetPolarity(polarity);
		} else if (param == "M") {
			Mirroring mirroring = ApertureTransformation::MirroringFromCommand(option);
			processor.GetTransformation().SetMirroring(mirroring);
		} else if (param == "R") {
			double rotation = std::stod(option);
			processor.GetTransformation().SetRotationDegrees(rotation);
		} else if (param == "S") {
			double scale = std::stod(option);
			processor.GetTransformation().SetScalingFactor(scale);
		}
	} else {
		throw std::invalid_argument("invalid aperture transformation");
	}
}

void CommandParser::HandleRegionStatement(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	(void)processor;
	(void)words;
	throw std::invalid_argument("command not implemented");
}

void CommandParser::HandleBlockAperture(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	(void)processor;
	(void)words;
	throw std::invalid_argument("command not implemented");
}

void CommandParser::HandleStepAndRepeat(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	(void)processor;
	(void)words;
	throw std::invalid_argument("command not implemented");
}

void CommandParser::HandleEndOfFile(CommandsProcessor &processor, const std::vector<std::string> &words) {
	AssertWordCommand(words);
	if (words[0] == "M02") {
		processor.SetEndOfFile();
	} else {
		throw std::logic_error("expected M02");
	}
}

