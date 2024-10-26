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

#include "Contour.h"
#include "MacroThermal.h"
#include "Serializer.h"
#include <cmath>
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
		throw std::invalid_argument("diameters and gap must be >= 0.0");
	}
	if (outerDiameter <= innerDiameter) {
		throw std::invalid_argument(
				"outer diameter must be larger than inner diameter");
	}
	if (gapThickness > outerDiameter / sqrt(2.0)) {
		throw std::invalid_argument(
				"gap thickness must be less than (outer diameter)/sqrt(2)");
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
	// Exposure is always ON
	double rOuter = 0.5 * transform.ApplyScaling(m_outerDiameter);
	double rInner = 0.5 * transform.ApplyScaling(m_innerDiameter);
	double dGap = 0.5 * transform.ApplyScaling(m_gapThickness);
	//TODO handle disappearing inner radius for large gap
	//r^2 = x^2 + y^2
	double dGapInner = sqrt(rInner * rInner - dGap * dGap);
	double dGapOuter = sqrt(rOuter * rOuter - dGap * dGap);

	Point innerBot(dGapInner, dGap);
	Point outerBot(dGapOuter, dGap);
	Point outerTop(dGap, dGapOuter);
	Point innerTop(dGap, dGapInner);

	for (int i = 0; i < 4; i++) {
		Contour contour;
		contour.AddSegment(std::make_shared<Segment>(innerBot, outerBot));
		contour.AddSegment(
				std::make_shared<ArcSegment>(outerBot, outerTop, -outerBot,
						ArcDirection::CounterClockwise));
		contour.AddSegment(std::make_shared<Segment>(outerTop, innerTop));
		contour.AddSegment(
				std::make_shared<ArcSegment>(innerTop, innerBot, -innerTop,
						ArcDirection::Clockwise));

		Transform rot;
		rot.SetRotation(m_rotation + 90.0 * i);
		contour.Transform(rot);
		contour.Translate(origin);
		serializer.AddContour(contour, isDark(transform));
	}
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
