/*
 * test_MacroTemplate.cpp
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
#include "CppUTest/TestHarness.h"

using namespace gerbex;

#define DBL_TOL 1e-5

template<typename T> std::shared_ptr<T> GetPrimitive(
		std::shared_ptr<Macro> macro, size_t index) {
	CHECK_TEXT(index < macro->GetPrimitives().size(),
			"no primitive at specified index");
	std::shared_ptr<MacroPrimitive> prim = macro->GetPrimitives().at(index);
	std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(prim);

	CHECK_TEXT(result != nullptr, "primitive is different type");
	return result;
}

std::shared_ptr<Macro> make_macro(const Fields &words,
		const Parameters &parameters) {
	MacroTemplate macroTemplate(words);
	std::shared_ptr<Aperture> aperture = macroTemplate.Call(parameters);
	std::shared_ptr<Macro> macro = std::dynamic_pointer_cast<Macro>(aperture);
	CHECK(macro != nullptr);

	return macro;
}

TEST_GROUP(MacroTemplateTest) {
};

TEST(MacroTemplateTest, Body) {
	Fields body = { "AMTHERMAL80", "7,0,0,0.800,0.550,0.125,45" };
	MacroTemplate macro(body);
	CHECK(body == macro.GetBody());
}

TEST(MacroTemplateTest, BadCode) {
	CHECK_THROWS(std::invalid_argument, make_macro( { "25,1,1.5,-3,+2" }, { }));
}

TEST(MacroTemplateTest, Comment) {
	std::shared_ptr<Macro> macro = make_macro( {
			"0 Rectangle with rounded corners, with rotation" }, { });
	CHECK(macro->GetPrimitives().empty());
}

TEST(MacroTemplateTest, Circle) {
	std::shared_ptr<Macro> macro = make_macro( { "1,1,1.5,-3,+2,45" }, { });
	std::shared_ptr<MacroCircle> circle = GetPrimitive<MacroCircle>(macro, 0);
	DOUBLES_EQUAL(1.5, circle->GetDiameter(), DBL_TOL);
}

TEST(MacroTemplateTest, VectorLine) {
	std::shared_ptr<Macro> macro = make_macro(
			{ "20,1,0.9,0,0.45,12,0.75,22.5" }, { });
	std::shared_ptr<MacroVectorLine> line = GetPrimitive<MacroVectorLine>(macro,
			0);
	DOUBLES_EQUAL(0.9, line->GetWidth(), DBL_TOL);
}

TEST(MacroTemplateTest, CenterLine) {
	std::shared_ptr<Macro> macro = make_macro( { "21,1,6.8,1.2,3.4,0.6,30" },
			{ });
	std::shared_ptr<MacroCenterLine> line = GetPrimitive<MacroCenterLine>(macro,
			0);
	DOUBLES_EQUAL(6.8, line->GetWidth(), DBL_TOL);
}

TEST(MacroTemplateTest, Outline) {
	std::shared_ptr<Macro> macro = make_macro( { "4,1,3,1,-1,1,1,2,1,1,-1,30" },
			{ });
	std::shared_ptr<MacroOutline> outline = GetPrimitive<MacroOutline>(macro,
			0);
	CHECK(MacroExposure::ON == outline->GetExposure());
	const std::vector<Point> vertices = outline->GetVertices();
	LONGS_EQUAL(4, vertices.size());
}

TEST(MacroTemplateTest, Polygon) {
	std::shared_ptr<Macro> macro = make_macro( { "5,1,8,3,4,6,12" }, { });
	std::shared_ptr<MacroPolygon> poly = GetPrimitive<MacroPolygon>(macro, 0);
	LONGS_EQUAL(8, poly->GetNumVertices());
}

TEST(MacroTemplateTest, Thermal) {
	std::shared_ptr<Macro> macro = make_macro(
			{ "7,0,0.25,0.95,0.75,0.175,22.5" }, { });
	std::shared_ptr<MacroThermal> thermal = GetPrimitive<MacroThermal>(macro,
			0);
	DOUBLES_EQUAL(0.95, thermal->GetOuterDiameter(), DBL_TOL);

}

TEST(MacroTemplateTest, FixedDonut) {
	std::shared_ptr<Macro> macro = make_macro( { "1,1,0.100,0,0",
			"1,0,0.080,0,0" }, { });
	std::shared_ptr<MacroCircle> outer = GetPrimitive<MacroCircle>(macro, 0);
	std::shared_ptr<MacroCircle> inner = GetPrimitive<MacroCircle>(macro, 1);
	CHECK(MacroExposure::ON == outer->GetExposure());
	DOUBLES_EQUAL(0.100, outer->GetDiameter(), DBL_TOL);
	CHECK(MacroExposure::OFF == inner->GetExposure());
	DOUBLES_EQUAL(0.080, inner->GetDiameter(), DBL_TOL);
}

TEST(MacroTemplateTest, VariableDonut) {
	std::shared_ptr<Macro> macro = make_macro(
			{ "1,1,$1,$2,$3", "1,0,$4,$2,$3" }, { 0.100, 0.02, 0.03, 0.080 });
	std::shared_ptr<MacroCircle> outer = GetPrimitive<MacroCircle>(macro, 0);
	std::shared_ptr<MacroCircle> inner = GetPrimitive<MacroCircle>(macro, 1);
	CHECK(MacroExposure::ON == outer->GetExposure());
	DOUBLES_EQUAL(0.100, outer->GetDiameter(), DBL_TOL);
	DOUBLES_EQUAL(0.02, outer->GetCenter().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.03, outer->GetCenter().GetY(), DBL_TOL);
	CHECK(MacroExposure::OFF == inner->GetExposure());
	DOUBLES_EQUAL(0.080, inner->GetDiameter(), DBL_TOL);
	DOUBLES_EQUAL(0.02, inner->GetCenter().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.03, inner->GetCenter().GetY(), DBL_TOL);
}

TEST(MacroTemplateTest, VariableDonut_TooFewParams) {
	CHECK_THROWS(std::invalid_argument, make_macro( { "1,1,$1,$2,$3",
			"1,0,$4,$2,$3" }, { 0.100, 0.0, 0.0 }));
}

TEST(MacroTemplateTest, NewVariable_Const) {
	std::shared_ptr<Macro> macro = make_macro( { "1,1,$1,$2,$3", "$4=0.015",
			"1,0,$4,$2,$3" }, { 0.02, 0.0, 0.0 });
	std::shared_ptr<MacroCircle> outer = GetPrimitive<MacroCircle>(macro, 0);
	std::shared_ptr<MacroCircle> inner = GetPrimitive<MacroCircle>(macro, 1);
	CHECK(MacroExposure::ON == outer->GetExposure());
	DOUBLES_EQUAL(0.020, outer->GetDiameter(), DBL_TOL);
	CHECK(MacroExposure::OFF == inner->GetExposure());
	DOUBLES_EQUAL(0.015, inner->GetDiameter(), DBL_TOL);
}

TEST(MacroTemplateTest, NewVariable_ArbitraryId) {
	//Variable id does not need to be in sequence
	std::shared_ptr<Macro> macro = make_macro( { "$17=0.015", "1,0,$17,$1,$2" },
			{ 0.01, 0.02 });
	std::shared_ptr<MacroCircle> circ = GetPrimitive<MacroCircle>(macro, 0);
	DOUBLES_EQUAL(0.015, circ->GetDiameter(), DBL_TOL);
	DOUBLES_EQUAL(0.01, circ->GetCenter().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.02, circ->GetCenter().GetY(), DBL_TOL);
}

TEST(MacroTemplateTest, NewVariable_Redefine) {
	// Cannot redefine variable
	CHECK_THROWS(std::invalid_argument, make_macro( { "1,1,$1,$2,$3",
			"$4=0.015", "$4=0.080" }, { 0.02, 0.0, 0.0 }));
}
