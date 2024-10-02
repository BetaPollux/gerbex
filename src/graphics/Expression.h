/*
 * Expression.h
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

#ifndef EXPRESSION_H_
#define EXPRESSION_H_

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>

namespace gerbex {

using Variables = std::unordered_map<int, double>;

class Operator {
public:
	Operator() {
	}
	virtual ~Operator() {
	}
	virtual double Evaluate(double left, double right) = 0;
	virtual int Precedence() = 0;
};

class Addition: public Operator {
public:
	double Evaluate(double left, double right) override {
		return left + right;
	}
	int Precedence() override {
		return 0;
	}
};

class Subtraction: public Operator {
public:
	double Evaluate(double left, double right) override {
		return left - right;
	}
	int Precedence() override {
		return 0;
	}
};

class Multiplication: public Operator {
public:
	double Evaluate(double left, double right) override {
		return left * right;
	}
	int Precedence() override {
		return 1;
	}
};

class Division: public Operator {
public:
	double Evaluate(double left, double right) override {
		return left / right;
	}
	int Precedence() override {
		return 1;
	}
};

/*
 *
 */
class Expression {
public:
	Expression();
	Expression(std::string body);
	virtual ~Expression();
	double Evaluate(const Variables &vars = {}) const;
	const std::string &GetBody() const;

private:
	static std::unique_ptr<Operator> MakeOperator(const std::string &op);
	static double LookupVariable(const std::string &id, const Variables &vars);
	static void ApplyOperator(std::stack<double> &output,
			std::stack<std::unique_ptr<Operator>> &operators);

private:
	std::string m_body;
};

} /* namespace gerbex */

#endif /* EXPRESSION_H_ */
