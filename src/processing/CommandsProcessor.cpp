/*
 * CommandsProcessor.cpp
 *
 *  Created on: Mar. 24, 2024
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

#include "Arc.h"
#include "BlockAperture.h"
#include "CircleTemplate.h"
#include "CommandsProcessor.h"
#include "Draw.h"
#include "Flash.h"
#include "ObroundTemplate.h"
#include "PolygonTemplate.h"
#include "RectangleTemplate.h"
#include "Region.h"
#include "StepAndRepeat.h"
#include <stdexcept>

namespace gerbex {

CommandsProcessor::CommandsProcessor() :
		m_commandState { CommandState::Normal }, m_graphicsState { }, m_objects { }, m_apertures { }, m_templates { }, m_activeRegion {
				nullptr }, m_openBlocks { 0 } {
	m_templates["C"] = std::make_unique<CircleTemplate>();
	m_templates["R"] = std::make_unique<RectangleTemplate>();
	m_templates["O"] = std::make_unique<ObroundTemplate>();
	m_templates["P"] = std::make_unique<PolygonTemplate>();
	m_objectDest.push(&m_objects);
}

CommandsProcessor::~CommandsProcessor() {
	m_apertures.clear();
	m_templates.clear();
}

void CommandsProcessor::ApertureDefine(int ident,
		std::shared_ptr<Aperture> aperture) {
	if (aperture == nullptr) {
		throw std::invalid_argument("cannot add null aperture");
	}
	if (ident < 10) {
		throw std::invalid_argument("aperture ident must be >= 10");
	}
	if (m_apertures.find(ident) == m_apertures.end()) {
		m_apertures[ident] = aperture;
	} else {
		throw std::invalid_argument("aperture ident already used");
	}
}

void CommandsProcessor::SetCurrentAperture(int ident) {
	if (m_apertures.find(ident) == m_apertures.end()) {
		throw std::invalid_argument(
				"aperture " + std::to_string(ident) + " does not exist");
	}
	m_graphicsState.SetCurrentAperture(m_apertures[ident]);
}

std::shared_ptr<Aperture> CommandsProcessor::GetAperture(int ident) const {
	auto result = m_apertures.find(ident);
	if (result == m_apertures.end()) {
		throw std::invalid_argument(
				"aperture " + std::to_string(ident) + " does not exist");
	}
	return result->second;
}

void CommandsProcessor::PlotDraw(const Point &coord) {
	if (m_graphicsState.GetPlotState() != PlotState::Linear) {
		throw std::logic_error("can only plot Draw in Linear plot state");
	}

	if (!m_graphicsState.GetCurrentPoint().has_value()) {
		throw std::logic_error("draw requires valid current point");
	}

	if (m_commandState != CommandState::InsideRegion
			&& m_graphicsState.GetCurrentAperture() == nullptr) {
		throw std::logic_error("draw requires valid current aperture");
	}

	std::shared_ptr<Draw> obj = std::make_shared<Draw>(
			*m_graphicsState.GetCurrentPoint(), coord,
			m_graphicsState.GetCurrentAperture(),
			m_graphicsState.GetTransformation());

	if (m_commandState != CommandState::InsideRegion) {
		m_objectDest.top()->push_back(obj);
	} else {
		m_activeRegion->AddSegment(obj);
	}

	m_graphicsState.SetCurrentPoint(coord);
}

void CommandsProcessor::PlotArc(const Point &coord, const Point &offset) {
	ArcDirection direction;

	if (m_graphicsState.GetPlotState() == PlotState::Clockwise) {
		direction = ArcDirection::Clockwise;
	} else if (m_graphicsState.GetPlotState() == PlotState::CounterClockwise) {
		direction = ArcDirection::CounterClockwise;
	} else {
		throw std::logic_error("can only plot Arc in CW or CCW plot state");
	}

	if (!m_graphicsState.GetCurrentPoint().has_value()) {
		throw std::logic_error("arc requires valid current point");
	}

	if (m_commandState != CommandState::InsideRegion
			&& m_graphicsState.GetCurrentAperture() == nullptr) {
		throw std::logic_error("arc requires valid current aperture");
	}

	std::shared_ptr<Arc> obj = std::make_shared<Arc>(
			*m_graphicsState.GetCurrentPoint(), coord, offset, direction,
			m_graphicsState.GetCurrentAperture(),
			m_graphicsState.GetTransformation());

	if (m_commandState != CommandState::InsideRegion) {
		m_objectDest.top()->push_back(obj);
	} else {
		m_activeRegion->AddSegment(obj);
	}

	m_graphicsState.SetCurrentPoint(coord);
}

void CommandsProcessor::Move(const Point &coord) {
	if (m_commandState == CommandState::InsideRegion) {
		m_activeRegion->StartContour();
	}
	m_graphicsState.SetCurrentPoint(coord);
}

void CommandsProcessor::Flash(const Point &coord) {
	if (m_graphicsState.GetCurrentAperture() == nullptr) {
		throw std::logic_error("flash requires defined current aperture");
	}

	std::unique_ptr<gerbex::Flash> obj = std::make_unique<gerbex::Flash>(coord,
			m_graphicsState.GetCurrentAperture(),
			m_graphicsState.GetTransformation());
	m_objectDest.top()->push_back(std::move(obj));
	m_graphicsState.SetCurrentPoint(coord);
}

void CommandsProcessor::AddTemplate(std::string name,
		std::shared_ptr<ApertureTemplate> new_tmpl) {
	if (m_templates.find(name) == m_templates.end()) {
		m_templates[name] = new_tmpl;
	} else {
		throw std::invalid_argument(
				"aperture template " + name + " already exists");
	}
}

std::shared_ptr<ApertureTemplate> CommandsProcessor::GetTemplate(
		const std::string &name) {
	auto result = m_templates.find(name);
	if (result == m_templates.end()) {
		throw std::invalid_argument(
				"aperture template " + name + " does not exist");
	}
	return result->second;
}

GraphicsState& CommandsProcessor::GetGraphicsState() {
	return m_graphicsState;
}
const std::vector<std::shared_ptr<GraphicalObject> >& CommandsProcessor::GetObjects() const {
	return m_objects;
}

CommandState CommandsProcessor::GetCommandState() const {
	return m_commandState;
}

void CommandsProcessor::SetEndOfFile() {
	m_commandState = CommandState::EndOfFile;
}

void CommandsProcessor::StartRegion() {
	if (m_commandState == CommandState::InsideRegion) {
		throw std::logic_error("cannot start a region inside a region");
	}
	m_activeRegion = std::make_unique<Region>(m_graphicsState.GetTransformation());
	m_commandState = CommandState::InsideRegion;
}

void CommandsProcessor::EndRegion() {
	if (m_commandState != CommandState::InsideRegion) {
		throw std::logic_error("cannot end region; not inside a region");
	}
	m_objectDest.top()->push_back(std::move(m_activeRegion));
	m_commandState = CommandState::Normal;
}

void CommandsProcessor::OpenApertureBlock(int ident) {
	std::shared_ptr<BlockAperture> block = std::make_shared<BlockAperture>();
	ApertureDefine(ident, block);
	m_objectDest.push(block->GetObjectList());
	m_openBlocks++;
}

void CommandsProcessor::CloseApertureBlock() {
	if (m_openBlocks > 0) {
		m_objectDest.pop();
		m_graphicsState.SetCurrentPoint(std::nullopt);
		m_openBlocks--;
	} else {
		throw std::logic_error("cannot close aperture block; none open");
	}
}

void CommandsProcessor::OpenStepAndRepeat(int nx, int ny, double dx,
		double dy) {
	if (m_activeStepAndRepeat == nullptr) {
		m_activeStepAndRepeat = std::make_unique<StepAndRepeat>(nx, ny, dx, dy);
		m_objectDest.push(m_activeStepAndRepeat->GetObjectList());
	} else {
		throw std::logic_error("cannot open step and repeat; already open");
	}
}

void CommandsProcessor::CloseStepAndRepeat() {
	if (m_activeStepAndRepeat != nullptr) {
		m_objectDest.pop();
		m_objectDest.top()->push_back(std::move(m_activeStepAndRepeat));
		m_graphicsState.SetCurrentPoint(std::nullopt);
	} else {
		throw std::logic_error("cannot close step and repeat; not opened");
	}
}

} /* namespace gerbex */
