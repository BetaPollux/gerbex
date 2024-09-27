/*
 * MacroTemplate.cpp
 *
 *  Created on: Mar. 30, 2024
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
#include "Macro.h"
#include "MacroCircle.h"
#include "MacroCenterLine.h"
#include "MacroOutline.h"
#include "MacroPolygon.h"
#include "MacroTemplate.h"
#include "MacroThermal.h"
#include "MacroVectorLine.h"
#include <stdexcept>

namespace gerbex {

MacroTemplate::MacroTemplate() {
	// Empty

}

MacroTemplate::MacroTemplate(Fields body)
	: m_body{ body }
{
	// Empty
}

MacroTemplate::~MacroTemplate() {
	// Empty
}


std::unique_ptr<Aperture> MacroTemplate::Call(const Parameters &parameters) {
	//TODO create Macro
	//TODO insert parameters into body
	//TODO parse body
	//TODO add primitives
	std::string word = *m_body.begin();
	std::unique_ptr<Macro> macro = std::make_unique<Macro>();
	int code = std::stoi(word.substr(0, 1));
	if (code != (int)MacroCodes::COMMENT) {
		Parameters params = DataTypeParser::SplitParams(word, ',');
		macro->AddPrimitive(MacroTemplate::MakeCircle(params));
	}
	return macro;
}

const Fields& MacroTemplate::GetBody() const {
	return m_body;
}

MacroExposure MacroTemplate::ExposureFromNum(int num) {
	switch (num) {
	case 1:
		return MacroExposure::ON;
	case 0:
		return MacroExposure::OFF;
	default:
		throw std::invalid_argument("macro exposure must be 0 or 1");
	}
}

std::unique_ptr<MacroPrimitive> MacroTemplate::HandleComment(
		const Parameters &params) {
	(void)params;
	return nullptr;
}

std::unique_ptr<MacroPrimitive> MacroTemplate::MakeCircle(
		const Parameters &params) {
	if (params.size() != 5) {
		throw std::invalid_argument("macro circle expects 5 parameters");
	}

	MacroExposure exposure = ExposureFromNum((int)params[0]);
	double diameter = params[1];
	RealPoint center(params[2], params[3]);
	double rotation = params[4];
	return std::make_unique<MacroCircle>(exposure, diameter, center, rotation);
}

#if 0
std::unique_ptr<MacroPrimitive> MacroTemplate::MakeCenterLine(
		const Parameters &params) {
	if (params.size() != 6) {
		throw std::invalid_argument("macro center line expects 6 parameters");
	}
	MacroExposure exposure = ExposureFromNum((int)params[0]);
	double width = params[1];
	double height = params[2];
	RealPoint center(params[3], params[4]);
	double rotation = params[5];
	return std::make_unique<MacroCenterLine>(exposure, width, height, center, rotation);
}

std::unique_ptr<MacroPrimitive> MacroTemplate::MakeVectorLine(
		const Parameters &params) {
	if (params.size() != 7) {
		throw std::invalid_argument("macro vector line expects 7 parameters");
	}
	MacroExposure exposure = ExposureFromNum((int)params[0]);
	double width = params[1];
	RealPoint start(params[2], params[3]);
	RealPoint end(params[4], params[5]);
	double rotation = params[6];
	return std::make_unique<MacroVectorLine>(exposure, width, start, end, rotation);
}

std::unique_ptr<MacroPrimitive> MacroTemplate::MakeOutline(
		const Parameters &params) {
	if (params.size() < 5) {
		throw std::invalid_argument("macro outline expects at least 5 parameters");
	}
	MacroExposure exposure = ExposureFromNum((int)params[0]);
	size_t num = (size_t)params[1] + 1;

	if (params.size() != (5 + 2 * (num - 1))) {
		throw std::invalid_argument("macro outline expects 5+2n parameters");
	}
	std::vector<RealPoint> vertices;
	vertices.reserve(num);
	auto it = params.begin() + 2;
	for (size_t i = 0; i < num; i++) {
		double x = *it++;
		double y = *it++;
		vertices.push_back(RealPoint(x, y));
	}
	double rotation = params.back();
	return std::make_unique<MacroOutline>(exposure, vertices, rotation);
}

std::unique_ptr<MacroPrimitive> MacroTemplate::MakePolygon(
		const Parameters &params) {
	if (params.size() != 6) {
		throw std::invalid_argument("macro polygon expects 6 parameters");
	}
	MacroExposure exposure = ExposureFromNum((int)params[0]);
	size_t num = (size_t)params[1];
	RealPoint center(params[2], params[3]);
	double diameter = params[4];
	double rotation = params[5];
	return std::make_unique<MacroPolygon>(exposure, num, center, diameter, rotation);
}

std::unique_ptr<MacroPrimitive> MacroTemplate::MakeThermal(
		const Parameters &params) {
	if (params.size() != 6) {
		throw std::invalid_argument("macro thermal expects 6 parameters");
	}
	RealPoint center(params[0], params[1]);
	double outer = params[2];
	double inner = params[3];
	double gap = params[4];
	double rotation = params[5];
	return std::make_unique<MacroThermal>(center, outer, inner, gap, rotation);
}

#endif

} /* namespace gerbex */
