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
#include <stdexcept>

CommandsProcessor::CommandsProcessor()
	: m_commandState{ CommandState::Normal },
	  m_graphicsState{},
	  m_objects{},
	  m_apertures{},
	  m_templates{},
	  m_activeRegion{ nullptr }
{
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
		throw std::invalid_argument("Cannot add null aperture.");
	}
	if (ident < 10) {
		throw std::invalid_argument("Aperture ident must be >= 10.");
	}
	m_apertures[ident] = aperture;
}

void CommandsProcessor::SetCurrentAperture(int ident) {
	if (m_apertures.count(ident) == 0) {
		throw std::invalid_argument("Aperture " + std::to_string(ident) + " does not exist.");
	}
	m_graphicsState.SetCurrentAperture(m_apertures[ident]);
}

void CommandsProcessor::PlotDraw(const Point &coord) {
	if (m_graphicsState.GetPlotState() != PlotState::Linear) {
		throw std::logic_error("Can only plot Draw in Linear plot state.");
	}

	if (m_graphicsState.GetCurrentPoint() == nullptr) {
		throw std::logic_error("Draw requires valid current point.");
	}

	if (m_commandState != CommandState::InsideRegion && m_graphicsState.GetCurrentAperture() == nullptr) {
		throw std::logic_error("Draw requires valid current aperture.");
	}

	std::shared_ptr<Draw> obj = std::make_shared<Draw>(
			*m_graphicsState.GetCurrentPoint(),
			coord,
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
		throw std::logic_error("Can only plot Arc in CW or CCW plot state.");
	}

	if (m_graphicsState.GetCurrentPoint() == nullptr) {
		throw std::logic_error("Arc requires valid current point.");
	}

	if (m_commandState != CommandState::InsideRegion && m_graphicsState.GetCurrentAperture() == nullptr) {
		throw std::logic_error("Arc requires valid current aperture.");
	}

	std::shared_ptr<Arc> obj = std::make_shared<Arc>(
			*m_graphicsState.GetCurrentPoint(),
			coord, offset, direction,
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
		throw std::logic_error("Flash requires defined current aperture");
	}

	//Flash requires explicit namespace due to same method name
	std::unique_ptr<::Flash> obj = std::make_unique<::Flash>(
			coord,
			m_graphicsState.GetCurrentAperture(),
			m_graphicsState.GetTransformation());
	m_objectDest.top()->push_back(std::move(obj));
	m_graphicsState.SetCurrentPoint(coord);
}

std::shared_ptr<ApertureTemplate> CommandsProcessor::GetTemplate(std::string name) {
	if (m_templates.count(name) == 0) {
		throw std::invalid_argument("Aperture template " + name + " does not exist.");
	}
	return m_templates[name];
}

const GraphicsState& CommandsProcessor::GetGraphicsState() const {
	return m_graphicsState;
}

const std::vector<std::shared_ptr<GraphicalObject> >& CommandsProcessor::GetObjects() const {
	return m_objects;
}

CommandState CommandsProcessor::GetCommandState() const {
	return m_commandState;
}

void CommandsProcessor::SetPlotState(PlotState state) {
	m_graphicsState.SetPlotState(state);
}

void CommandsProcessor::StartRegion() {
	if (m_commandState == CommandState::InsideRegion){
		throw std::logic_error("Cannot start a region inside a region.");
	}
	m_activeRegion = std::make_unique<Region>();
	m_commandState = CommandState::InsideRegion;
}

void CommandsProcessor::EndRegion() {
	if (m_commandState != CommandState::InsideRegion){
		throw std::logic_error("Can't end region; not inside a region");
	}
	m_objectDest.top()->push_back(std::move(m_activeRegion));
	m_commandState = CommandState::Normal;
}

void CommandsProcessor::OpenApertureBlock(int ident) {
	std::shared_ptr<BlockAperture> block = std::make_shared<BlockAperture>();
	ApertureDefine(ident, block);
	m_objectDest.push(block->GetObjects());
}

void CommandsProcessor::CloseApertureBlock() {
	//TODO confirm there is an open AB to close
	m_objectDest.pop();
}

void CommandsProcessor::SetCommandState(CommandState commandState) {
	m_commandState = commandState;
}

