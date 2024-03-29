/*
 * GraphicsState.cpp
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

#include "GraphicsState.h"
#include <stdexcept>

GraphicsState::GraphicsState()
	: m_format{ nullptr },
	  m_unit{ Unit::Invalid },
	  m_currentPoint{ nullptr },
	  m_currentAperture{ nullptr },
	  m_plotState{ PlotState::Invalid },
	  m_transformation{}
{
	// Empty

}

GraphicsState::~GraphicsState() {
	// Empty
}

const std::shared_ptr<Aperture> GraphicsState::GetCurrentAperture() const {
	return m_currentAperture;
}

void GraphicsState::SetCurrentAperture(std::shared_ptr<Aperture> currentAperture) {
	m_currentAperture = currentAperture;
}

const std::shared_ptr<Point> GraphicsState::GetCurrentPoint() const {
	return m_currentPoint;
}

void GraphicsState::SetCurrentPoint(std::shared_ptr<Point> currentPoint) {
	m_currentPoint = currentPoint;
}

void GraphicsState::SetCurrentPoint(const Point &currentPoint) {
	m_currentPoint = std::make_shared<Point>(currentPoint);
}

const std::shared_ptr<CoordinateFormat> GraphicsState::GetFormat() const {
	return m_format;
}

void GraphicsState::SetFormat(std::shared_ptr<CoordinateFormat> format) {
	m_format = format;
}

void GraphicsState::SetFormat(const CoordinateFormat &format) {
	m_format = std::make_shared<CoordinateFormat>(format);
}

PlotState GraphicsState::GetPlotState() const {
	return m_plotState;
}

void GraphicsState::SetPlotState(PlotState plotState) {
	m_plotState = plotState;
}

const ApertureTransformation& GraphicsState::GetTransformation() const {
	return m_transformation;
}

void GraphicsState::SetTransformation(
		const ApertureTransformation &transformation) {
	m_transformation = transformation;
}

Unit GraphicsState::GetUnit() const {
	return m_unit;
}

void GraphicsState::SetUnit(Unit unit) {
	m_unit = unit;
}

