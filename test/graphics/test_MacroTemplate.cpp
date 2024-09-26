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

template <typename T> std::shared_ptr<T> GetPrimitive(std::shared_ptr<MacroPrimitive> prim) {
	std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(prim);

	CHECK(result != nullptr);

	return result;
}

TEST_GROUP(MacroTemplateTest) {
};

TEST(MacroTemplateTest, ExposureFromStr) {
	CHECK(MacroExposure::OFF == MacroTemplate::ExposureFromNum(0));
	CHECK(MacroExposure::ON == MacroTemplate::ExposureFromNum(1));
	CHECK_THROWS(std::invalid_argument, MacroTemplate::ExposureFromNum(2));
}

TEST(MacroTemplateTest, Body) {
	std::list<std::string> body = {"AMTHERMAL80", "7,0,0,0.800,0.550,0.125,45" };
	MacroTemplate macro(body);
	CHECK(body == macro.GetBody());
}

TEST(MacroTemplateTest, Comment) {
	std::string word = "0 Rectangle with rounded corners, with rotation";
	std::shared_ptr<MacroPrimitive> prim = MacroTemplate::HandleComment(word);
	POINTERS_EQUAL(nullptr, prim.get());
}

TEST(MacroTemplateTest, Circle) {
	std::string word = "1,1.5,-3,+2,45";
	std::shared_ptr<MacroPrimitive> prim = MacroTemplate::MakeCircle(word);

	std::shared_ptr<MacroCircle> circle = GetPrimitive<MacroCircle>(prim);
	CHECK(MacroExposure::ON == circle->GetExposure());
	DOUBLES_EQUAL(1.5, circle->GetDiameter(), DBL_TOL);
	DOUBLES_EQUAL(-3.0, circle->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(2.0, circle->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(45.0, circle->GetRotation(), DBL_TOL);
}

TEST(MacroTemplateTest, Circle_TooFewParams) {
	std::string word = "1,1.5,-3,+2";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeCircle(word));
}

TEST(MacroTemplateTest, Circle_TooManyParams) {
	std::string word = "1,1.5,-3,+2,45,23";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeCircle(word));
}

TEST(MacroTemplateTest, VectorLine) {
	std::string word = "1,0.9,0,0.45,12,0.75,22.5";
	std::shared_ptr<MacroPrimitive> prim = MacroTemplate::MakeVectorLine(word);

	std::shared_ptr<MacroVectorLine> line = GetPrimitive<MacroVectorLine>(prim);
	CHECK(MacroExposure::ON == line->GetExposure());
	DOUBLES_EQUAL(0.9, line->GetWidth(), DBL_TOL);
	DOUBLES_EQUAL(0.0, line->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.45, line->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(12.0, line->GetEnd().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.75, line->GetEnd().GetY(), DBL_TOL);
	DOUBLES_EQUAL(22.5, line->GetRotation(), DBL_TOL);
}

TEST(MacroTemplateTest, VectorLine_TooFewParams) {
	std::string word = "1,0.9,0,0.45,12,0.45";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeVectorLine(word));
}

TEST(MacroTemplateTest, VectorLine_TooManyParams) {
	std::string word = "1,0.9,0,0.45,12,0.45,0,1";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeVectorLine(word));
}

TEST(MacroTemplateTest, CenterLine) {
	std::string word = "1,6.8,1.2,3.4,0.6,30";
	std::shared_ptr<MacroPrimitive> prim = MacroTemplate::MakeCenterLine(word);

	std::shared_ptr<MacroCenterLine> line = GetPrimitive<MacroCenterLine>(prim);
	CHECK(MacroExposure::ON == line->GetExposure());
	DOUBLES_EQUAL(6.8, line->GetWidth(), DBL_TOL);
	DOUBLES_EQUAL(1.2, line->GetHeight(), DBL_TOL);
	DOUBLES_EQUAL(3.4, line->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.6, line->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(30.0, line->GetRotation(), DBL_TOL);
}

TEST(MacroTemplateTest, CenterLine_TooFewParams) {
	std::string word = "1,6.8,1.2,3.4,0.6";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeCenterLine(word));
}

TEST(MacroTemplateTest, CenterLine_TooManyParams) {
	std::string word = "1,6.8,1.2,3.4,0.6,30,5";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeCenterLine(word));
}

TEST(MacroTemplateTest, Outline) {
	std::string word = "1,3,1,-1,1,1,2,1,1,-1,30";
	std::shared_ptr<MacroPrimitive> prim = MacroTemplate::MakeOutline(word);

	std::shared_ptr<MacroOutline> outline = GetPrimitive<MacroOutline>(prim);
	CHECK(MacroExposure::ON == outline->GetExposure());
	const std::vector<RealPoint> vertices = outline->GetVertices();
	LONGS_EQUAL(4, vertices.size());
	CHECK(outline->GetCoord() == vertices[0]);
	DOUBLES_EQUAL(1.0, vertices[0].GetX(), DBL_TOL);
	DOUBLES_EQUAL(-1.0, vertices[0].GetY(), DBL_TOL);
	DOUBLES_EQUAL(1.0, vertices[1].GetX(), DBL_TOL);
	DOUBLES_EQUAL(1.0, vertices[1].GetY(), DBL_TOL);
	DOUBLES_EQUAL(2.0, vertices[2].GetX(), DBL_TOL);
	DOUBLES_EQUAL(1.0, vertices[2].GetY(), DBL_TOL);
	DOUBLES_EQUAL(1.0, vertices[3].GetX(), DBL_TOL);
	DOUBLES_EQUAL(-1.0, vertices[3].GetY(), DBL_TOL);
	DOUBLES_EQUAL(30.0, outline->GetRotation(), DBL_TOL);
}

TEST(MacroTemplateTest, Outline_TooFewParams) {
	std::string word = "1";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeOutline(word));
}

TEST(MacroTemplateTest, Outline_TooFewXY) {
	std::string word = "1,4,1,-1,1,1,2,1,1,-1,30"; // Less X,Y than vertices param
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeOutline(word));
}

TEST(MacroTemplateTest, Outline_TooManyXY) {
	std::string word = "1,2,1,-1,1,1,2,1,1,-1,30"; // More X,Y than vertices param
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeOutline(word));
}

TEST(MacroTemplateTest, Polygon) {
	std::string word = "1,8,3,4,6,12";
	std::shared_ptr<MacroPrimitive> prim = MacroTemplate::MakePolygon(word);

	std::shared_ptr<MacroPolygon> poly = GetPrimitive<MacroPolygon>(prim);
	CHECK(MacroExposure::ON == poly->GetExposure());
	LONGS_EQUAL(8, poly->GetNumVertices());
	DOUBLES_EQUAL(3.0, poly->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(4.0, poly->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(6.0, poly->GetDiameter(), DBL_TOL);
	DOUBLES_EQUAL(12.0, poly->GetRotation(), DBL_TOL);
}

TEST(MacroTemplateTest, Polygon_TooFewParams) {
	std::string word = "1,8,0,0,8";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakePolygon(word));
}

TEST(MacroTemplateTest, Polygon_TooManyParams) {
	std::string word = "1,8,0,0,8,0,1.0";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakePolygon(word));
}

TEST(MacroTemplateTest, Thermal) {
	std::string word = "0,0.25,0.95,0.75,0.175,22.5";
	std::shared_ptr<MacroPrimitive> prim = MacroTemplate::MakeThermal(word);

	std::shared_ptr<MacroThermal> thermal = GetPrimitive<MacroThermal>(prim);
	CHECK(MacroExposure::ON == thermal->GetExposure());
	DOUBLES_EQUAL(0.0, thermal->GetCoord().GetX(), DBL_TOL);
	DOUBLES_EQUAL(0.25, thermal->GetCoord().GetY(), DBL_TOL);
	DOUBLES_EQUAL(0.95, thermal->GetOuterDiameter(), DBL_TOL);
	DOUBLES_EQUAL(0.75, thermal->GetInnerDiameter(), DBL_TOL);
	DOUBLES_EQUAL(0.175, thermal->GetGapThickness(), DBL_TOL);
	DOUBLES_EQUAL(22.5, thermal->GetRotation(), DBL_TOL);
}

TEST(MacroTemplateTest, Thermal_TooFewParams) {
	std::string word = "0,0,0.95,0.75,0.175";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeThermal(word));
}

TEST(MacroTemplateTest, Thermal_TooManyParams) {
	std::string word = "0,0,0.95,0.75,0.175,0.0,5.0";
	CHECK_THROWS(std::invalid_argument, MacroTemplate::MakeThermal(word));
}

