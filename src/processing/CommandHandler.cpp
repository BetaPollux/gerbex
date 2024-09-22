/*
 * CommandHandler.cpp
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

#include "CommandHandler.h"
#include "CoordinateData.h"
#include "DataTypeParser.h"
#include <regex>
#include <stdexcept>

namespace gerbex {

void CommandHandler::AssertWordCommand(const std::vector<std::string> &words) {
	if (words.size() != 1) {
		throw std::invalid_argument("expected word command, got extended command");
	}
}

void CommandHandler::AssertOperationsCode(const std::string &word, const std::string &expected) {
	std::string code = word.substr(word.size() - 3, 3);
	if (code != expected) {
		throw std::runtime_error("expected " + code + ", got " + expected);
	}
}

void CommandHandler::NotImplemented(CommandsProcessor &processor, const std::vector<std::string> &words) {
	(void)processor;
	(void)words;
	throw std::invalid_argument("command not implemented");
}

void CommandHandler::Comment(CommandsProcessor &processor, const std::vector<std::string> &words) {
	(void)processor;
	AssertWordCommand(words);
	// Ignore comment
}

void CommandHandler::Unit(CommandsProcessor &processor, const std::vector<std::string> &words) {
	AssertWordCommand(words);
	processor.SetUnit(GraphicsState::UnitFromCommand(words[0]));
}

void CommandHandler::Format(CommandsProcessor &processor, const std::vector<std::string> &words) {
	AssertWordCommand(words);
	processor.SetFormat(CoordinateFormat::FromCommand(words[0]));
}

void CommandHandler::ApertureDefine(CommandsProcessor &processor,
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
		std::vector<double> params = DataTypeParser::SplitParams(match[3].str());
		std::shared_ptr<ApertureTemplate> aperture = processor.GetTemplate(name);
		processor.ApertureDefine(ident, aperture->Call(params));
	} else {
		throw std::invalid_argument("invalid aperture define");
	}
}

void CommandHandler::ApertureMacro(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	(void)processor;
	(void)words;
	throw std::invalid_argument("command not implemented");
}

void CommandHandler::SetCurrentAperture(CommandsProcessor &processor,
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

void CommandHandler::PlotState(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);

	gerbex::PlotState state = GraphicsState::PlotStateFromCommand(words[0]);
	processor.SetPlotState(state);
}

void CommandHandler::Plot(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	AssertOperationsCode(words[0], "D01");

	CoordinateData coorddata = CoordinateData::FromString(words[0]);
	std::optional<Point> coord = coorddata.GetXY(processor.GetCurrentPoint());
	std::optional<Point> offset = coorddata.GetIJ();

	switch (processor.GetPlotState()) {
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
	}
}

void CommandHandler::Move(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	AssertOperationsCode(words[0], "D02");

	CoordinateData coorddata = CoordinateData::FromString(words[0]);
	std::optional<Point> coord = coorddata.GetXY(processor.GetCurrentPoint());

	if (coorddata.GetIJ().has_value()) {
		throw std::invalid_argument("Move cannot have offset coordinate");
	}

	processor.Move(*coord);
}

void CommandHandler::Flash(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	AssertOperationsCode(words[0], "D03");

	CoordinateData coorddata = CoordinateData::FromString(words[0]);
	std::optional<Point> coord = coorddata.GetXY(processor.GetCurrentPoint());

	if (coorddata.GetIJ().has_value()) {
		throw std::invalid_argument("Flash cannot have offset coordinate");
	}

	processor.Flash(*coord);
}

void CommandHandler::ApertureTransformations(CommandsProcessor &processor,
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

void CommandHandler::RegionStatement(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	if (words[0] == "G36") {
		processor.StartRegion();
	} else if (words[0] == "G37") {
		processor.EndRegion();
	} else {
		throw std::invalid_argument("invalid region statement");
	}
}

void CommandHandler::BlockAperture(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	DataTypeParser parser;
	std::string uint_re = parser.GetPattern("unsigned_integer");
	std::ostringstream pattern;
	pattern << "AB(D(" << uint_re << "))?";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words[0], match, regex)) {
		if (!match[1].str().empty()) {
			//Has ident, open block
			int ident = std::stoi(match[2].str());
			processor.OpenApertureBlock(ident);
		} else {
			//No ident, close block
			processor.CloseApertureBlock();
		}
	} else {
		throw std::invalid_argument("invalid block aperture statement");
	}
}

void CommandHandler::StepAndRepeat(CommandsProcessor &processor,
		const std::vector<std::string> &words) {
	AssertWordCommand(words);
	DataTypeParser parser;
	std::string pint_re = parser.GetPattern("positive_integer");
	std::string dec_re = parser.GetPattern("unsigned_decimal");
	std::ostringstream pattern;
	pattern << "SR(";
	pattern << "X(" << pint_re << ")";
	pattern << "Y(" << pint_re << ")";
	pattern << "I(" << dec_re << ")";
	pattern << "J(" << dec_re << ")";
	pattern << ")?";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words[0], match, regex)) {
		if (!match[1].str().empty()) {
			//Has params, open step/repeat
			int nx = std::stoi(match[2].str());
			int ny = std::stoi(match[3].str());
			double dx = std::stod(match[4].str());
			double dy = std::stod(match[5].str());
			processor.OpenStepAndRepeat(nx, ny, dx, dy);
		} else {
			//No ident, close step/repeat
			processor.CloseStepAndRepeat();
		}
	} else {
		throw std::invalid_argument("invalid step and repeat statement");
	}
}

void CommandHandler::EndOfFile(CommandsProcessor &processor, const std::vector<std::string> &words) {
	AssertWordCommand(words);
	if (words[0] == "M02") {
		processor.SetEndOfFile();
	} else {
		throw std::logic_error("expected M02");
	}
}

} /* namespace gerbex */
