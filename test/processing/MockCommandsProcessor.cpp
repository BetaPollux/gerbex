/*
 * MockCommandsProcessor.cpp
 *
 *  Created on: Sep. 22, 2024
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

#include "MockTemplate.h"
#include "MockCommandsProcessor.h"
#include "CppUTestExt/MockSupport.h"

namespace gerbex {

MockCommandsProcessor::MockCommandsProcessor() {
}

MockCommandsProcessor::~MockCommandsProcessor() {
}

void MockCommandsProcessor::StartRegion() {
	mock().actualCall("StartRegion");
}

void MockCommandsProcessor::EndRegion() {
	mock().actualCall("EndRegion");
}

std::shared_ptr<ApertureTemplate> MockCommandsProcessor::GetTemplate(
		std::string name) {
	return *(std::shared_ptr<ApertureTemplate>*) mock().actualCall(
			"GetTemplate").withParameter("name", name.c_str()).returnPointerValue();
}

void MockCommandsProcessor::OpenApertureBlock(int ident) {
	mock().actualCall("OpenApertureBlock").withParameter("ident", ident);
}

void MockCommandsProcessor::CloseApertureBlock() {
	mock().actualCall("CloseApertureBlock");
}

void MockCommandsProcessor::OpenStepAndRepeat(int nx, int ny, double dx,
		double dy) {
	mock().actualCall("OpenStepAndRepeat").withParameter("nx", nx).withParameter(
			"ny", ny).withParameter("dx", dx).withParameter("dy", dy);
}

void MockCommandsProcessor::CloseStepAndRepeat() {
	mock().actualCall("CloseStepAndRepeat");
}

void MockCommandsProcessor::SetEndOfFile() {
	mock().actualCall("SetEndOfFile");
}

void MockCommandsProcessor::ApertureDefine(int ident,
		std::shared_ptr<Aperture> aperture) {
	mock().actualCall("ApertureDefine").withParameter("ident", ident).withParameter(
			"aperture", aperture.get());
}

std::shared_ptr<Aperture> MockCommandsProcessor::GetAperture(int ident) const {
	mock().actualCall("GetAperture").withParameter("ident", ident);
	return *(std::shared_ptr<Aperture>*) mock().actualCall("GetCurrentPoint").returnPointerValue();
}

void MockCommandsProcessor::PlotDraw(const Point &coord) {
	mock().actualCall("PlotDraw").withParameterOfType("Point", "coord", &coord);
}

void MockCommandsProcessor::PlotArc(const Point &coord, const Point &offset) {
	mock().actualCall("PlotArc").withParameterOfType("Point", "coord", &coord).withParameterOfType(
			"Point", "offset", &offset);
}

CommandState MockCommandsProcessor::GetCommandState() const {
	return (CommandState) mock().actualCall("GetCommandState").returnIntValue();
}

void MockCommandsProcessor::Flash(const Point &coord) {
	mock().actualCall("Flash").withParameterOfType("Point", "coord", &coord);
}

const std::vector<std::shared_ptr<GraphicalObject>>& MockCommandsProcessor::GetObjects() const {
	return *(std::vector<std::shared_ptr<GraphicalObject>>*) mock().actualCall(
			"GetObjects").returnPointerValue();
}

void MockCommandsProcessor::Move(const Point &coord) {
	mock().actualCall("Move").withParameterOfType("Point", "coord", &coord);
}


GraphicsState& MockCommandsProcessor::GetGraphicsState() {
	return *(GraphicsState*) mock().actualCall("GetGraphicsState").returnPointerValue();
}

} /* namespace gerbex */
