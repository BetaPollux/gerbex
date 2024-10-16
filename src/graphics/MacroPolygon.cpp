/*
 * MacroPolygon.cpp
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

#include "MacroPolygon.h"
#include "Serializer.h"
#include <cmath>
#include <stdexcept>

namespace gerbex {

MacroPolygon::MacroPolygon() :
		MacroPolygon(MacroExposure::ON, 6, Point(0.0, 0.0), 1.0, 0.0) {
	// Empty
}

MacroPolygon::MacroPolygon(MacroExposure exposure, int numVertices,
		const Point &center, double diameter, double rotation) :
		MacroPrimitive(exposure, rotation), m_center { center }, m_numVertices { numVertices }, m_diameter {
				diameter } {
	if (numVertices < 3 || numVertices > 12) {
		throw std::invalid_argument("Number of vertices must be from 3 to 12");
	}

	if (diameter < 0.0) {
		throw std::invalid_argument("Diameter must be >= 0.0");
	}
}

MacroPolygon::~MacroPolygon() {
	// Empty
}

double MacroPolygon::GetDiameter() const {
	return m_diameter;
}

int MacroPolygon::GetNumVertices() const {
	return m_numVertices;
}

std::unique_ptr<MacroPolygon> MacroPolygon::FromParameters(
		const Parameters &params) {
	if (params.size() != 6) {
		throw std::invalid_argument("macro polygon expects 6 parameters");
	}
	MacroExposure exposure = MacroPrimitive::ExposureFromNum((int) params[0]);
	size_t num = (size_t) params[1];
	Point center(params[2], params[3]);
	double diameter = params[4];
	double rotation = params[5];
	return std::make_unique<MacroPolygon>(exposure, num, center, diameter,
			rotation);
}

void MacroPolygon::Serialize(gerbex::Serializer &serializer) {
	//Regular polygon
	std::vector<Point> points;
	double angle_step = 2.0 * M_PI / m_numVertices;
	for (int i = 0; i < m_numVertices; i++) {
		double angle = angle_step * i;
		double x = 0.5 * m_diameter * cos(angle);
		double y = 0.5 * m_diameter * sin(angle);
		points.push_back( { x, y });
	}
	serializer.PushPolarity(m_exposure);
	serializer.AddPolygon(points);
	serializer.PopPolarity();
}

const Point& MacroPolygon::GetCenter() const {
	return m_center;
}

} /* namespace gerbex */
