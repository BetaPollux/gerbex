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
	Variables variables = MacroTemplate::GetVariables(parameters);
	std::unique_ptr<Macro> macro = std::make_unique<Macro>();
	for (std::string &block : m_body) {
		std::smatch match;
		std::regex regex("^(([0-9]+)|([$][0-9]+=))");
		MacroCodes code;
		if (std::regex_search(block, match, regex)) {
			if (match[2].matched) {
				// Statement, primitive code
				code = (MacroCodes) std::stoi(match[1].str());
			} else {
				// Variable definition, $x=
				MacroTemplate::DefineVariable(block, variables);
				continue;
			}
		} else {
			throw std::invalid_argument("invalid macro body");
		}
		if (code == MacroCodes::COMMENT) {
			continue;
		}
		MacroTemplate::InsertVariables(block, variables);
		Parameters prim_params = DataTypeParser::SplitParams(block, ',');
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
			throw std::invalid_argument("invalid macro code " + block);
		}
	}
	return macro;
}

const Fields& MacroTemplate::GetBody() const {
	return m_body;
}

Variables MacroTemplate::GetVariables(const Parameters &parameters) {
	// Variables start with $1
	Variables vars;
	for (size_t i = 0; i < parameters.size(); i++) {
		vars[i + 1] = parameters[i];
	}
	return vars;
}

void MacroTemplate::InsertVariables(std::string &block, const Variables &vars) {
	std::smatch match;
	std::regex var_re("[$]([0-9]+)");
	while (std::regex_search(block, match, var_re)) {
		int var_id = std::stoi(match[1].str());
		auto value = vars.find(var_id);
		if (value != vars.end()) {
			std::regex repl_re("[$]" + std::to_string(var_id));
			std::string val_str = std::to_string(value->second);
			block = std::regex_replace(block, repl_re, val_str);
		} else {
			throw std::invalid_argument(
					"variable " + match.str()
							+ " was not provided in macro call");
		}
	}
}

void MacroTemplate::DefineVariable(std::string &block, Variables &vars) {
	// TODO handle expressions support + - x / ( )
	std::smatch match;
	std::regex regex(
			"^[$]([0-9]+)=(" + DataTypeParser::GetNumberPattern() + ")");
	if (std::regex_search(block, match, regex)) {
		int var_id = std::stoi(match[1].str());
		if (vars.find(var_id) == vars.end()) {
			vars[var_id] = std::stod(match[2].str());
		} else {
			throw std::invalid_argument(
					"variable $" + std::to_string(var_id)
							+ " cannot be redefined");
		}
	} else {
		throw std::invalid_argument("macro expressions not yet supported");
	}
}

} /* namespace gerbex */
