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
#include "MacroTemplate.h"
#include <iostream>
#include <regex>
#include <stdexcept>

namespace gerbex {

void CommandHandler::AssertWordCommand(const Fields &words) {
	if (words.size() != 1) {
		throw std::invalid_argument(
				"expected word command, got extended command");
	}
}

void CommandHandler::AssertCommandCode(const std::string &word,
		const std::string &expected) {
	std::string code = DataTypeParser::GetCommandCode(word);
	if (code != expected) {
		throw std::runtime_error("expected " + code + ", got " + expected);
	}
}

void CommandHandler::NotImplemented(CommandsProcessor &processor,
		Fields &words) {
	(void) processor;
	(void) words;
	throw std::invalid_argument("command not implemented");
}

void CommandHandler::Comment(CommandsProcessor &processor, Fields &words) {
	(void) processor;
	AssertWordCommand(words);
	// Ignore comment
}

void CommandHandler::Unit(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);
	processor.GetGraphicsState().SetUnit(
			GraphicsState::UnitFromCommand(words.front()));
}

void CommandHandler::Format(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);
	processor.GetGraphicsState().SetFormat(
			CoordinateFormat::FromCommand(words.front()));
}

void CommandHandler::ApertureDefine(CommandsProcessor &processor,
		Fields &words) {
	AssertWordCommand(words);

	std::ostringstream pattern;
	pattern << "ADD(" << DataTypeParser::GetNumberPattern() << ")";
	pattern << "(" << DataTypeParser::GetNamePattern() << ")";
	pattern << "(,(" << DataTypeParser::GetFieldPattern() << "))?";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words.front(), match, regex)) {
		int ident = std::stoi(match[1].str());
		std::string name = match[2].str();
		Parameters params = DataTypeParser::SplitParams(match[4].str(), 'X');
		std::shared_ptr<ApertureTemplate> aperture = processor.GetTemplate(
				name);
		processor.ApertureDefine(ident, aperture->Call(params));
	} else {
		throw std::invalid_argument("invalid aperture define");
	}
}

void CommandHandler::ApertureMacro(CommandsProcessor &processor,
		Fields &words) {
	std::string pattern = "AM(" + DataTypeParser::GetNamePattern() + ")";
	std::regex regex(pattern);
	std::smatch match;
	if (std::regex_search(words.front(), match, regex)) {
		std::string name = match[1].str();
		words.pop_front();
		std::shared_ptr<MacroTemplate> macro = std::make_shared<MacroTemplate>(
				words);
		processor.AddTemplate(name, macro);
	} else {
		throw std::invalid_argument("invalid aperture macro");
	}
}

void CommandHandler::SetCurrentAperture(CommandsProcessor &processor,
		Fields &words) {
	AssertWordCommand(words);
	std::smatch match;
	std::regex regex("D(" + DataTypeParser::GetNumberPattern() + ")");
	if (std::regex_search(words.front(), match, regex)) {
		int ident = std::stoi(match[1].str());
		processor.SetCurrentAperture(ident);
	}
}

void CommandHandler::PlotState(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);

	gerbex::PlotState state = GraphicsState::PlotStateFromCommand(
			words.front());
	processor.GetGraphicsState().SetPlotState(state);
}

void CommandHandler::Plot(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);
	AssertCommandCode(words.front(), "D01");

	GraphicsState &graphicsState = processor.GetGraphicsState();

	CoordinateData coorddata = CoordinateData::FromString(words.front());
	Point coord = graphicsState.GetPoint(coorddata);
	graphicsState.AssertPlotState();

	switch (*graphicsState.GetPlotState()) {
	case PlotState::Linear:
		coorddata.AssertNoIJ();
		processor.PlotDraw(coord);
		break;
	case PlotState::Clockwise:
	case PlotState::CounterClockwise:
		FixedPoint fOffset = coorddata.GetIJChecked();
		graphicsState.AssertArcMode();
		Point offset = graphicsState.GetFormat()->Convert(fOffset);
		processor.PlotArc(coord, offset);
		break;
	}
}

void CommandHandler::Move(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);
	AssertCommandCode(words.front(), "D02");

	CoordinateData coorddata = CoordinateData::FromString(words.front());
	coorddata.AssertNoIJ();
	Point coord = processor.GetGraphicsState().GetPoint(coorddata);

	processor.Move(coord);
}

void CommandHandler::Flash(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);
	AssertCommandCode(words.front(), "D03");

	CoordinateData coorddata = CoordinateData::FromString(words.front());
	coorddata.AssertNoIJ();
	Point coord = processor.GetGraphicsState().GetPoint(coorddata);

	processor.Flash(coord);
}

void CommandHandler::ApertureTransformations(CommandsProcessor &processor,
		Fields &words) {
	AssertWordCommand(words);

	std::string num_re = DataTypeParser::GetNumberPattern();
	std::ostringstream pattern;
	pattern << "L([PMRS])";
	pattern << "([CDNXY]+|" << num_re << ")";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words.front(), match, regex)) {
		std::string param = match[1].str();
		std::string option = match[2].str();
		Transform &transformation =
				processor.GetGraphicsState().GetTransformation();
		if (param == "P") {
			Polarity polarity = Transform::PolarityFromCommand(
					option);
			transformation.SetPolarity(polarity);
		} else if (param == "M") {
			Mirroring mirroring = Transform::MirroringFromCommand(
					option);
			transformation.SetMirroring(mirroring);
		} else if (param == "R") {
			double rotation = std::stod(option);
			transformation.SetRotation(rotation);
		} else if (param == "S") {
			double scale = std::stod(option);
			transformation.SetScaling(scale);
		}
	} else {
		throw std::invalid_argument("invalid aperture transformation");
	}
}

void CommandHandler::RegionStatement(CommandsProcessor &processor,
		Fields &words) {
	AssertWordCommand(words);
	if (words.front() == "G36") {
		processor.StartRegion();
	} else if (words.front() == "G37") {
		processor.EndRegion();
	} else {
		throw std::invalid_argument("invalid region statement");
	}
}

void CommandHandler::BlockAperture(CommandsProcessor &processor,
		Fields &words) {
	AssertWordCommand(words);

	std::ostringstream pattern;
	pattern << "AB(D(" << DataTypeParser::GetNumberPattern() << "))?";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words.front(), match, regex)) {
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
		Fields &words) {
	AssertWordCommand(words);

	std::string num_re = DataTypeParser::GetNumberPattern();
	std::ostringstream pattern;
	pattern << "SR(";
	pattern << "X(" << num_re << ")";
	pattern << "Y(" << num_re << ")";
	pattern << "I(" << num_re << ")";
	pattern << "J(" << num_re << ")";
	pattern << ")?";

	std::regex regex(pattern.str());
	std::smatch match;
	if (std::regex_search(words.front(), match, regex)) {
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

void CommandHandler::EndOfFile(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);
	AssertCommandCode(words.front(), "M02");
	processor.SetEndOfFile();
}

void CommandHandler::ArcMode(CommandsProcessor &processor, Fields &words) {
	AssertWordCommand(words);
	gerbex::ArcMode mode = GraphicsState::ArcModeFromCommand(words.front());
	processor.GetGraphicsState().SetArcMode(mode);
}

} /* namespace gerbex */
