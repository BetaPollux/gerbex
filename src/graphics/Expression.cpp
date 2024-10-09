/*
 * Expression.cpp
 *
 *  Created on: Oct. 1, 2024
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

#include "Expression.h"
#include <regex>
#include <sstream>
#ifdef DEBUG_MACRO
	#include <iostream>
#endif

namespace gerbex {

Operator::Operator(char op) :
		m_op { op } {
}

Operator::~Operator() {
}

void Operator::Apply(std::vector<double> &output) {
	double right = output.back();
	output.pop_back();
	double left = 0.0;
	if (!output.empty()) {
		left = output.back();
		output.pop_back();
	}
	double result;
	switch (m_op) {
	case '+':
		result = left + right;
		break;
	case '-':
		result = left - right;
		break;
	case 'x':
		result = left * right;
		break;
	case '/':
		result = left / right;
		break;
	default:
		throw std::invalid_argument("unrecognized operator " + m_op);
	}
	output.push_back(result);
}

char Operator::OpChar() {
	return m_op;
}

int Operator::Precedence() {
	switch (m_op) {
	case '+':
	case '-':
		return 0;
	case 'x':
	case '/':
		return 1;
	default:
		throw std::invalid_argument("unrecognized operator " + m_op);
	}
}

Expression::Expression() :
		Expression("0") {
	// Empty
}

Expression::Expression(std::string body) :
		m_body { body } {
	// Empty
}

Expression::~Expression() {
	// Empty
}

const std::string& Expression::GetBody() const {
	return m_body;
}

double Expression::Evaluate(const Variables &vars) const {
	// Shunting yard algorithm
	std::vector<double> output;
	std::vector<std::shared_ptr<Operator>> operators;
	std::ostringstream pattern;
	pattern << "(";
	pattern << "([0-9]*[.]?[0-9]+)";	// Normal number
	pattern << "|([$][0-9]+)";			// Variable
	pattern << "|([()+x/-])";			// Operator (- must be last!)
	pattern << "|(\\S+)";				// Other invalid chars
	pattern << ")";
	std::regex re(pattern.str());
#ifdef DEBUG_MACRO
	std::cout << m_body << "\n";
#endif
	auto tokens_begin = std::sregex_iterator(m_body.begin(), m_body.end(), re);
	auto tokens_end = std::sregex_iterator();
	std::vector<size_t> open_brackets_idx;
	for (auto it = tokens_begin; it != tokens_end; ++it) {
		std::smatch match = *it;
		if (match[2].matched) {
			// Number
			output.push_back(std::stod(match.str()));
		} else if (match[3].matched) {
			// Variable
			output.push_back(LookupVariable(match.str(), vars));
		} else if (match[4].matched) {
			// Operator
			if (match.str() == "(") {
				// Mark where open bracket is
				open_brackets_idx.push_back(operators.size());
			} else if (match.str() == ")") {
				// Process all operators until open bracket
				if (open_brackets_idx.empty()) {
					throw std::invalid_argument("close bracket without open");
				}
				while (operators.size() > open_brackets_idx.back()) {
					ApplyOperator(output, operators);
				}
				open_brackets_idx.pop_back();
			} else {
				std::shared_ptr<Operator> new_op = MakeOperator(match.str());
				while (!operators.empty()
						&& new_op->Precedence()
								<= operators.back()->Precedence()) {
					// Process all higher precedence operators first
					if (!open_brackets_idx.empty()
							&& operators.size() == open_brackets_idx.back()) {
						break;
					}
					ApplyOperator(output, operators);
				}
				operators.push_back(std::move(new_op));
			}
		} else {
			throw std::invalid_argument("unrecognized tokens");
		}
#ifdef DEBUG_MACRO
		std::cout << "Op: ";
		for (auto op : operators) {
			std::cout << op->OpChar() << " ";
		}
		std::cout << "Out: ";
		for (auto out : output) {
			std::cout << out << " ";
		}
		std::cout << "Brk: ";
		for (auto br : open_brackets_idx) {
			std::cout << br << " ";
		}
		std::cout << std::endl;
#endif
	}
	while (!operators.empty()) {
		// Process remaining operators
		ApplyOperator(output, operators);
	}
	if (!open_brackets_idx.empty()) {
		throw std::invalid_argument("open bracket without close");
	}

	if (output.size() == 1) {
		return output.back();
	} else {
		throw std::invalid_argument("failed to process expression");
	}
}

std::unique_ptr<Operator> Expression::MakeOperator(const std::string &op) {
	return std::make_unique<Operator>(op[0]);
}

void Expression::ApplyOperator(std::vector<double> &output,
		std::vector<std::shared_ptr<Operator>> &operators) {
	if (output.empty()) {
		throw std::invalid_argument("missing operand");
	}
	if (operators.empty()) {
		throw std::invalid_argument("missing operator");
	}

	operators.back()->Apply(output);
	operators.pop_back();
}

double Expression::LookupVariable(const std::string &id,
		const Variables &vars) {
	std::smatch match;
	std::regex var_re("[$]([0-9]+)");
	if (std::regex_search(id, match, var_re)) {
		int var_id = std::stoi(match[1].str());
		auto value = vars.find(var_id);
		if (value != vars.end()) {
			return value->second;
		} else {
			throw std::invalid_argument(
					"variable " + match.str()
							+ " was not provided in macro call");
		}
	} else {
		throw std::invalid_argument("invalid variable id " + id);
	}
}

} /* namespace gerbex */
