/*
 * test_Expression.cpp
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
#include <stdexcept>
#include "CppUTest/TestHarness.h"

#define DBL_TOL	1e-5

namespace gerbex {

TEST_GROUP(ExpressionTest) {
};

TEST(ExpressionTest, Const) {
	Expression expr("7");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(7.0, result, DBL_TOL);
}

TEST(ExpressionTest, NegativeConst) {
	Expression expr("-5");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(-5.0, result, DBL_TOL);
}

TEST(ExpressionTest, PositiveConst) {
	Expression expr("+2");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(2.0, result, DBL_TOL);
}

TEST(ExpressionTest, RejectsBadChars) {
	CHECK_THROWS(std::invalid_argument, Expression("7a").Evaluate());
	CHECK_THROWS(std::invalid_argument, Expression("1o2").Evaluate());
	CHECK_THROWS(std::invalid_argument, Expression("k2").Evaluate());
}

TEST(ExpressionTest, Add) {
	Expression expr("7.00+3.00");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(10.0, result, DBL_TOL);
}

TEST(ExpressionTest, AddSeries) {
	Expression expr("1+2+3+4");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(10.0, result, DBL_TOL);
}

TEST(ExpressionTest, Subtract) {
	Expression expr("7-3");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(4.0, result, DBL_TOL);
}

TEST(ExpressionTest, AddSubtractSeries) {
	Expression expr("1-2+3-4");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(-2.0, result, DBL_TOL);
}

TEST(ExpressionTest, Multiply) {
	Expression expr("7x3");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(21.0, result, DBL_TOL);
}

TEST(ExpressionTest, MultiplySeries) {
	Expression expr("2x3x4");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(24.0, result, DBL_TOL);
}

TEST(ExpressionTest, Divide) {
	Expression expr("7.00/0.2");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(35.0, result, DBL_TOL);
}

TEST(ExpressionTest, MultiplyDivideSeries) {
	Expression expr("2x3/4");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(1.5, result, DBL_TOL);
}

TEST(ExpressionTest, MultiplyPrecedence) {
	Expression expr("2+3x4");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(14.0, result, DBL_TOL);
}

TEST(ExpressionTest, DividePrecedence) {
	Expression expr("1+4/2");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(3.0, result, DBL_TOL);
}

TEST(ExpressionTest, Compound) {
	Expression expr("1+2x3-4");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(3.0, result, DBL_TOL);
}

TEST(ExpressionTest, Brackets) {
	Expression expr("(7+3)x0.25");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(2.5, result, DBL_TOL);
}

TEST(ExpressionTest, NestedBrackets) {
	Expression expr("(2x(3-1))x4");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(16.0, result, DBL_TOL);
}

TEST(ExpressionTest, MissingOpenBracket) {
	Expression expr("3-1)x4");
	CHECK_THROWS(std::invalid_argument, expr.Evaluate());
}

TEST(ExpressionTest, MissingCloseBracket) {
	Expression expr("(3-1x4");
	CHECK_THROWS(std::invalid_argument, expr.Evaluate());
}

TEST(ExpressionTest, NegatedBrackets) {
	Expression expr("-(1+2)");
	double result = expr.Evaluate();
	DOUBLES_EQUAL(-3.0, result, DBL_TOL);
}

TEST(ExpressionTest, BracketsWithVariables) {
	Variables vars = {{1, 0.1}, {2, 0.05}, {3, 0.02}, {4, 0.01}};
	Expression expr("(-$1+3x$3)/2+$4");
	double result = expr.Evaluate(vars);
	DOUBLES_EQUAL(-0.01, result, DBL_TOL);
}

TEST(ExpressionTest, Variable) {
	Variables vars = {{2, 1.25}};
	Expression expr("$2");
	double result = expr.Evaluate(vars);
	DOUBLES_EQUAL(1.25, result, DBL_TOL);
}

TEST(ExpressionTest, AddVariables) {
	Variables vars = {{1, 0.75}, {2, 1.25}};
	Expression expr("$1+$2");
	double result = expr.Evaluate(vars);
	DOUBLES_EQUAL(2.0, result, DBL_TOL);
}

TEST(ExpressionTest, Variable_NotFound) {
	Variables vars = {{1, 0.0}};
	Expression expr("$2");
	CHECK_THROWS(std::invalid_argument, expr.Evaluate(vars));
}

TEST(ExpressionTest, PlusVariable) {
	Variables vars = {{2, 1.25}};
	Expression expr("+$2");
	double result = expr.Evaluate(vars);
	DOUBLES_EQUAL(1.25, result, DBL_TOL);
}

TEST(ExpressionTest, MinusVariable) {
	Variables vars = {{2, 1.25}};
	Expression expr("-$2");
	double result = expr.Evaluate(vars);
	DOUBLES_EQUAL(-1.25, result, DBL_TOL);
}

} /* namespace gerbex */
