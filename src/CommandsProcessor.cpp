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
#include "Circle.h"
#include "CommandsProcessor.h"
#include "Draw.h"
#include "Flash.h"
#include "Obround.h"
#include "Polygon.h"
#include "Rectangle.h"
#include <stdexcept>

CommandsProcessor::CommandsProcessor()
	: m_commandState{ CommandState::Normal },
	  m_graphicsState{},
	  m_objects{},
	  m_apertures{},
	  m_templates{}
{
	m_templates["C"] = std::make_unique<Circle>();
	m_templates["R"] = std::make_unique<Rectangle>();
	m_templates["O"] = std::make_unique<Obround>();
	m_templates["P"] = std::make_unique<Polygon>();
}

CommandsProcessor::~CommandsProcessor() {
	m_apertures.clear();
	m_templates.clear();
}

void CommandsProcessor::ApertureDefinition(int ident,
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

	if (m_graphicsState.GetCurrentAperture() == nullptr) {
		throw std::logic_error("Draw requires valid current aperture.");
	}

	std::unique_ptr<Draw> obj = std::make_unique<Draw>(
			*m_graphicsState.GetCurrentPoint(),
			coord,
			m_graphicsState.GetCurrentAperture(),
			m_graphicsState.GetTransformation());
	m_objects.push_back(std::move(obj));
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

	if (m_graphicsState.GetCurrentAperture() == nullptr) {
		throw std::logic_error("Arc requires valid current aperture.");
	}

	std::unique_ptr<Arc> obj = std::make_unique<Arc>(
			*m_graphicsState.GetCurrentPoint(),
			coord, offset, direction,
			m_graphicsState.GetCurrentAperture(),
			m_graphicsState.GetTransformation());
	m_objects.push_back(std::move(obj));
	m_graphicsState.SetCurrentPoint(coord);
}

void CommandsProcessor::Move(const Point &coord) {
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
	m_objects.push_back(std::move(obj));
	m_graphicsState.SetCurrentPoint(coord);
}

std::shared_ptr<Aperture> CommandsProcessor::GetTemplate(std::string name) {
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

void CommandsProcessor::SetCommandState(CommandState commandState) {
	m_commandState = commandState;
}
