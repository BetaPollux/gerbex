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

namespace gerbex {

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

double Expression::Evaluate() const {
	// Shunting yard algorithm
	std::stack<double> output;
	std::stack<std::unique_ptr<Operator>> operators;
	std::string num_re = "[0-9]*[.]?[0-9]+";
	std::ostringstream pattern;
	pattern << "(";
	pattern << "(^[+-]" + num_re + ")|";	// Leading number with unary + or -
	pattern << "(" + num_re + ")|";			// Normal number
	pattern << "([+-x/])";					// Operator
	pattern << ")";
	std::regex re(pattern.str());
	auto tokens_begin = std::sregex_iterator(m_body.begin(), m_body.end(), re);
	auto tokens_end = std::sregex_iterator();
	for (auto it = tokens_begin; it != tokens_end; ++it) {
		std::smatch match = *it;
		std::string label;
		if (match[2].matched || match[3].matched) {
			// Number
			output.push(std::stod(match.str()));
		} else if (match[4].matched) {
			// Operator
			std::unique_ptr<Operator> new_op = MakeOperator(match.str());
			while (!operators.empty()
					&& new_op->Precedence() <= operators.top()->Precedence()) {
				// Process all higher precedence operators first
				ApplyOperator(output, operators);
			}
			operators.push(std::move(new_op));
		} else {
			throw std::invalid_argument("failed to parse expression");
		}
	}
	while (!operators.empty()) {
		// Process remaining operators
		ApplyOperator(output, operators);
	}
	return output.top();
}

std::unique_ptr<Operator> Expression::MakeOperator(const std::string &op) {
	std::unique_ptr<Operator> result;
	if (op == "+") {
		result = std::make_unique<Addition>();
	} else if (op == "-") {
		result = std::make_unique<Subtraction>();
	} else if (op == "x") {
		result = std::make_unique<Multiplication>();
	} else if (op == "/") {
		result = std::make_unique<Division>();
	} else {
		throw std::invalid_argument("invalid operator " + op);
	}
	return result;
}

void Expression::ApplyOperator(std::stack<double> &output,
		std::stack<std::unique_ptr<Operator>> &operators) {
	if (output.size() < 2) {
		throw std::invalid_argument("missing operand");
	}
	if (operators.empty()) {
		throw std::invalid_argument("missing operator");
	}

	double right = output.top();
	output.pop();
	double left = output.top();
	output.pop();
	double result = operators.top()->Evaluate(left, right);
	operators.pop();
	output.push(result);
}

} /* namespace gerbex */
