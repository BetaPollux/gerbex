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
	m_commandState = CommandState::Normal;
	m_graphicsState.SetPlotState(PlotState::Linear);
}

MockCommandsProcessor::~MockCommandsProcessor() {
}


void MockCommandsProcessor::EndRegion() {
	mock().actualCall("EndRegion");
}

std::shared_ptr<ApertureTemplate> MockCommandsProcessor::GetTemplate(
		std::string name) {
	mock().actualCall("GetTemplate").withParameter("name", name.c_str());
	return std::make_shared<MockTemplate>();
}

void MockCommandsProcessor::OpenApertureBlock(int ident) {
}

void MockCommandsProcessor::SetEndOfFile() {
}

const GraphicsState& MockCommandsProcessor::GetGraphicsState() const {
	return m_graphicsState;
}

void MockCommandsProcessor::SetUnit(Unit unit) {
}

void MockCommandsProcessor::PlotArc(const Point &coord,
		const Point &offset) {
}

CommandState MockCommandsProcessor::GetCommandState() const {
	return m_commandState;
}

void MockCommandsProcessor::Flash(const Point &coord) {
}

const std::vector<std::shared_ptr<GraphicalObject>,
		std::allocator<std::shared_ptr<GraphicalObject> > >& MockCommandsProcessor::GetObjects() const {
}

void MockCommandsProcessor::CloseStepAndRepeat() {
}

void MockCommandsProcessor::CloseApertureBlock() {
}

void MockCommandsProcessor::SetFormat(const CoordinateFormat &format) {
}

void MockCommandsProcessor::Move(const Point &coord) {
}

std::shared_ptr<Aperture> MockCommandsProcessor::GetAperture(
		int ident) const {
}

void MockCommandsProcessor::OpenStepAndRepeat(int nx, int ny, double dx,
		double dy) {
}

void MockCommandsProcessor::StartRegion() {
	mock().actualCall("StartRegion");
}

void MockCommandsProcessor::PlotDraw(const Point &coord) {
	mock().actualCall("PlotDraw").withParameterOfType("Point", "coord", &coord);
}

void MockCommandsProcessor::ApertureDefine(int ident,
		std::shared_ptr<Aperture> aperture) {
	mock().actualCall("ApertureDefine").withParameter("ident", ident);
}

void MockCommandsProcessor::SetPlotState(PlotState state) {
}

void MockCommandsProcessor::SetCurrentAperture(int ident) {
}

ApertureTransformation& MockCommandsProcessor::GetTransformation() {
	return m_apertureTransformation;
}

} /* namespace gerbex */
