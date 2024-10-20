/*
 * MacroCircle.cpp
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

#include "MacroCircle.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

MacroCircle::MacroCircle() :
		MacroCircle(MacroExposure::ON, 1.0, Point(0.0, 0.0), 0.0) {
	// Empty
}

MacroCircle::MacroCircle(MacroExposure exposure, double diameter,
		const Point &center, double rotation) :
		MacroPrimitive(exposure, rotation), m_center { center }, m_diameter {
				diameter } {
	if (diameter < 0.0) {
		throw std::invalid_argument("Diameter must be >= 0.0");
	}
}

double MacroCircle::GetDiameter() const {
	return m_diameter;
}

std::unique_ptr<MacroCircle> MacroCircle::FromParameters(
		const Parameters &params) {
	if (params.size() < 4 || params.size() > 5) {
		throw std::invalid_argument("macro circle expects 4 to 5 parameters");
	}

	MacroExposure exposure = MacroPrimitive::ExposureFromNum((int) params[0]);
	double diameter = params[1];
	Point center(params[2], params[3]);
	double rotation;
	if (params.size() > 4) {
		rotation = params[4];
	} else {
		rotation = 0.0;
	}
	return std::make_unique<MacroCircle>(exposure, diameter, center, rotation);
}

void MacroCircle::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	double radius = 0.5 * transform.ApplyScaling(m_diameter);
	Point center = transform.Apply(getRotatedCenter());
	center += origin;
	serializer.AddCircle(radius, center, isDark(transform));
}

const Point& MacroCircle::GetCenter() const {
	return m_center;
}

Box MacroCircle::GetBox() const {
	return Box(m_diameter, getRotatedCenter());
}

Point MacroCircle::getRotatedCenter() const {
	Point c = m_center;
	c.Rotate(m_rotation);
	return c;
}

} /* namespace gerbex */
