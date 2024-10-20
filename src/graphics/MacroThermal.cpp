/*
 * MacroThermal.cpp
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

#include "MacroThermal.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

MacroThermal::MacroThermal() :
		MacroThermal(Point(0.0, 0.0), 1.0, 0.5, 0.25, 0.0) {
	// Empty
}

MacroThermal::MacroThermal(const Point &center, double outerDiameter,
		double innerDiameter, double gapThickness, double rotation) :
		MacroPrimitive(MacroExposure::ON, rotation), m_center { center }, m_outerDiameter {
				outerDiameter }, m_innerDiameter { innerDiameter }, m_gapThickness {
				gapThickness } {
	if (outerDiameter < 0.0 || innerDiameter < 0.0 || gapThickness < 0.0) {
		throw std::invalid_argument("Diameters and gap must be >= 0.0");
	}
	if (outerDiameter <= innerDiameter) {
		throw std::invalid_argument(
				"Outer diameter must be larger than inner diameter");
	}
	if (gapThickness > outerDiameter / 1.4142) {
		throw std::invalid_argument(
				"Gap thickness must be less than (outer diameter)/sqrt(2)");
	}
}

double MacroThermal::GetGapThickness() const {
	return m_gapThickness;
}

double MacroThermal::GetInnerDiameter() const {
	return m_innerDiameter;
}

double MacroThermal::GetOuterDiameter() const {
	return m_outerDiameter;
}

std::unique_ptr<MacroThermal> MacroThermal::FromParameters(
		const Parameters &params) {
	if (params.size() != 6) {
		throw std::invalid_argument("macro thermal expects 6 parameters");
	}
	Point center(params[0], params[1]);
	double outer = params[2];
	double inner = params[3];
	double gap = params[4];
	double rotation = params[5];
	return std::make_unique<MacroThermal>(center, outer, inner, gap, rotation);
}

void MacroThermal::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	//TODO need to draw thermal
	// Exposure is always ON
	double radius = 0.5 * transform.ApplyScaling(m_outerDiameter);
	Point center = transform.Apply(getRotatedCenter());
	center += origin;
	serializer.AddCircle(radius, center, isDark(transform));
}

const Point& MacroThermal::GetCenter() const {
	return m_center;
}

Box MacroThermal::GetBox() const {
	return Box(m_outerDiameter, getRotatedCenter());
}

Point MacroThermal::getRotatedCenter() const {
	Point c = m_center;
	c.Rotate(m_rotation);
	return c;
}

} /* namespace gerbex */
