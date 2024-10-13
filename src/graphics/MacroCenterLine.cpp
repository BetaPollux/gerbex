/*
 * MacroCenterLine.cpp
 *
 *  Created on: Apr. 1, 2024
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

#include "MacroCenterLine.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

MacroCenterLine::MacroCenterLine() :
		MacroCenterLine(MacroExposure::ON, 1.0, 0.5, Point(0.0, 0.0), 0.0) {
	// Empty
}

MacroCenterLine::MacroCenterLine(MacroExposure exposure, double width,
		double height, const Point &start, double rotation) :
		MacroPrimitive(exposure, start, rotation), m_width { width }, m_height {
				height } {
	if (width < 0.0 || height < 0.0) {
		throw std::invalid_argument("Width and height must be >= 0.0");
	}
}

MacroCenterLine::~MacroCenterLine() {
	// Empty
}

double MacroCenterLine::GetHeight() const {
	return m_height;
}

double MacroCenterLine::GetWidth() const {
	return m_width;
}

std::unique_ptr<MacroCenterLine> MacroCenterLine::FromParameters(
		const Parameters &params) {
	if (params.size() != 6) {
		throw std::invalid_argument("macro center line expects 6 parameters");
	}
	MacroExposure exposure = MacroPrimitive::ExposureFromNum((int) params[0]);
	double width = params[1];
	double height = params[2];
	Point center(params[3], params[4]);
	double rotation = params[5];
	return std::make_unique<MacroCenterLine>(exposure, width, height, center,
			rotation);
}

void MacroCenterLine::Serialize(gerbex::Serializer &serializer) {
	if (m_rotation == 0.0) {
		serializer.AddRectangle(m_width, m_height,
				m_coord - Point(m_width, m_height) * 0.5);
	} else {
		//TODO use a vector line instead?
		double dx = 0.5 * m_width;
		double dy = 0.5 * m_height;
		std::vector<Point> corners = { Point(dx, dy), Point(-dx, dy), Point(-dx,
				-dy), Point(dx, -dy) };
		for (Point &pt : corners) {
			pt = pt + m_coord;
			pt.Rotate(m_rotation);
		}
		serializer.AddPolygon(corners);
	}
}

} /* namespace gerbex */
