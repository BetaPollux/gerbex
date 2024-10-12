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
#include <iostream>
#include <stdexcept>

namespace gerbex {

GraphicsState::GraphicsState() :
		m_format { }, m_unit { }, m_currentPoint { }, m_currentAperture { }, m_plotState { }, m_transformation { } {
	// Empty

}

GraphicsState::~GraphicsState() {
	// Empty
}

const std::shared_ptr<Aperture> GraphicsState::GetCurrentAperture() const {
	return m_currentAperture;
}

void GraphicsState::SetCurrentAperture(std::shared_ptr<Aperture> aperture) {
	m_currentAperture = aperture;
}

std::optional<Point> GraphicsState::GetCurrentPoint() const {
	return m_currentPoint;
}

void GraphicsState::SetCurrentPoint(std::optional<Point> point) {
	m_currentPoint = point;
}

std::optional<CoordinateFormat> GraphicsState::GetFormat() const {
	return m_format;
}

void GraphicsState::SetFormat(std::optional<CoordinateFormat> format) {
	m_format = format;
}

std::optional<PlotState> GraphicsState::GetPlotState() const {
	return m_plotState;
}

void GraphicsState::SetPlotState(std::optional<PlotState> plotState) {
	m_plotState = plotState;
}

ApertureTransformation& GraphicsState::GetTransformation() {
	return m_transformation;
}

void GraphicsState::SetTransformation(
		const ApertureTransformation &transformation) {
	m_transformation = transformation;
}

std::optional<Unit> GraphicsState::GetUnit() const {
	return m_unit;
}

void GraphicsState::SetUnit(std::optional<Unit> unit) {
	m_unit = unit;
}

Unit GraphicsState::UnitFromCommand(const std::string &str) {
	if (str == "MOMM") {
		return Unit::Millimeter;
	} else if (str == "MOIN") {
		return Unit::Inch;
	}
	throw std::invalid_argument("invalid unit");
}

PlotState GraphicsState::PlotStateFromCommand(const std::string &str) {
	if (str == "G01") {
		return PlotState::Linear;
	} else if (str == "G02") {
		return PlotState::Clockwise;
	} else if (str == "G03") {
		return PlotState::CounterClockwise;
	} else {
		throw std::invalid_argument("invalid plot state");
	}
}

std::optional<ArcMode> GraphicsState::GetArcMode() const {
	return m_arcMode;
}

void GraphicsState::SetArcMode(std::optional<ArcMode> arcMode) {
	if (arcMode == ArcMode::SingleQuadrant) {
		throw std::invalid_argument(
				"single quadrant arc mode is not supported");
	}
	m_arcMode = arcMode;
}

ArcMode GraphicsState::ArcModeFromCommand(const std::string &str) {
	if (str == "G75") {
		return ArcMode::MultiQuadrant;
	} else if (str == "G74") {
		return ArcMode::SingleQuadrant;
	} else {
		throw std::invalid_argument("invalid arc mode");
	}
}

Point GraphicsState::GetPoint(const CoordinateData &data) const {
	if (!data.HasXY() && !m_currentPoint.has_value()) {
		throw std::invalid_argument("coordinate data is not fully defined");
	}

	std::optional<PointType> newX, newY;
	if (data.GetX().has_value()) {
		newX = m_format->Convert(*data.GetX());
	}
	if (data.GetY().has_value()) {
		newY = m_format->Convert(*data.GetY());
	}

	return Point(newX.value_or(m_currentPoint->GetX()),
			newY.value_or(m_currentPoint->GetY()));
}

void GraphicsState::AssertPlotState() {
	if (!m_plotState.has_value()) {
		SetPlotState(gerbex::PlotState::Linear);
		std::cerr << "WARNING plot state was not defined, assuming linear"
				<< std::endl;
	}
}

void GraphicsState::AssertArcMode() {
	if (!m_arcMode.has_value()) {
		SetArcMode(gerbex::ArcMode::MultiQuadrant);
		std::cerr << "WARNING arc mode was not defined, assuming multi-quadrant"
				<< std::endl;
	}
}

} /* namespace gerbex */

