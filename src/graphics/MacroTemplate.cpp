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
#include <regex>
#include <stdexcept>

namespace gerbex {

MacroTemplate::MacroTemplate() {
	// Empty

}

MacroTemplate::MacroTemplate(Fields body) :
		m_body { body } {
	// Empty
}

MacroTemplate::~MacroTemplate() {
	// Empty
}

std::unique_ptr<Aperture> MacroTemplate::Call(const Parameters &parameters) {
	(void) parameters;
	//TODO handle parameters and expressions
	std::unique_ptr<Macro> macro = std::make_unique<Macro>();
	for (std::string &w : m_body) {
		std::smatch match;
		std::regex regex("^([0-9]+)?([$][0-9]+)?");
		MacroCodes code;
		if (std::regex_search(w, match, regex)) {
			if (match[1].matched) {
				code = (MacroCodes) std::stoi(match[1].str());
			} else {
				throw std::invalid_argument(
						"macro variable definiton is not yet supported");
				continue;
			}
		}
		if (code == MacroCodes::COMMENT) {
			continue;
		}
		MacroTemplate::InsertVariables(w, parameters);
		Parameters prim_params = DataTypeParser::SplitParams(w, ',');
		prim_params.pop_front();	// Discard code
		switch (code) {
		case MacroCodes::CIRCLE:
			macro->AddPrimitive(MacroCircle::FromParameters(prim_params));
			break;
		case MacroCodes::VECTOR_LINE:
			macro->AddPrimitive(MacroVectorLine::FromParameters(prim_params));
			break;
		case MacroCodes::CENTER_LINE:
			macro->AddPrimitive(MacroCenterLine::FromParameters(prim_params));
			break;
		case MacroCodes::OUTLINE:
			macro->AddPrimitive(MacroOutline::FromParameters(prim_params));
			break;
		case MacroCodes::POLYGON:
			macro->AddPrimitive(MacroPolygon::FromParameters(prim_params));
			break;
		case MacroCodes::THERMAL:
			macro->AddPrimitive(MacroThermal::FromParameters(prim_params));
			break;
		default:
			throw std::invalid_argument("invalid macro code " + w);
		}
	}
	return macro;
}

const Fields& MacroTemplate::GetBody() const {
	return m_body;
}

void MacroTemplate::InsertVariables(std::string &block,
		const Parameters &params) {
	(void)params;
	std::smatch match;
	std::regex regex("[$]([0-9]+)");
	if (std::regex_search(block, match, regex)) {
		throw std::invalid_argument("macro variables are not yet supported");

	}
}

} /* namespace gerbex */
