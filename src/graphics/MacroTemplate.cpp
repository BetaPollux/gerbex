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
#include "Expression.h"
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
			if (match[3].matched) {
				// Variable definition, $x=
				MacroTemplate::DefineVariable(block, variables);
				continue;
			}
		} else {
			throw std::invalid_argument("invalid macro body");
		}
		// Statement, starting with primitive code
		std::deque<Expression> expr = SplitStatement(block);
		code = (MacroCodes) std::stoi(expr.front().GetBody());
		if (code == MacroCodes::COMMENT) {
			continue;
		}
		expr.pop_front();	// Discard code
		Parameters prim_params = ProcessExpressions(expr, variables);
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

void MacroTemplate::DefineVariable(std::string &block, Variables &vars) {
	std::smatch match;
	std::regex regex(
			"[$]([0-9]+)=([^%*,]+)");
	if (std::regex_match(block, match, regex)) {
		int var_id = std::stoi(match[1].str());
		if (vars.find(var_id) == vars.end()) {
			Expression expr(match[2].str());
			vars[var_id] = expr.Evaluate(vars);
		} else {
			throw std::invalid_argument(
					"variable $" + std::to_string(var_id)
							+ " cannot be redefined");
		}
	} else {
		throw std::invalid_argument("invalid variable definition");
	}
}

std::deque<Expression> MacroTemplate::SplitStatement(std::string &block) {
	std::deque<Expression> expr;
	if (block.empty()) {
		return expr;
	}

	std::istringstream istr(block);
	while (!istr.eof()) {
		std::string expr_str;
		std::getline(istr, expr_str, ',');
		expr.push_back(Expression(expr_str));
	}

	return expr;
}

Parameters MacroTemplate::ProcessExpressions(std::deque<Expression> &expr, const Variables &vars) {
	Parameters params;
	for (auto e: expr) {
		double p = e.Evaluate(vars);
		params.push_back(p);
	}
	return params;
}

} /* namespace gerbex */
