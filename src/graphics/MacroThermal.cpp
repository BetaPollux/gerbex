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
#include <cmath>
#include <stdexcept>

namespace gerbex {

MacroThermal::MacroThermal() :
		MacroThermal(Point(0.0, 0.0), 1.0, 0.5, 0.25, 0.0) {
	// Empty
}

MacroThermal::MacroThermal(const Point &center, double outerDiameter,
		double innerDiameter, double gapThickness, double rotation) :
		MacroPrimitive(MacroExposure::ON), m_contours { } {
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

	// Exposure is always ON
	double rOuter = 0.5 * outerDiameter;
	double rInner = 0.5 * innerDiameter;
	double dGap = 0.5 * gapThickness;
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
		rot.SetRotation(rotation + 90.0 * i);
		contour.Transform(rot);
		contour.Translate(center);
		m_contours[i] = contour;
	}
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

void MacroThermal::Serialize(Serializer &serializer,
		const Point &origin) const {
	for (const Contour &c : m_contours) {
		Contour clone = c;
		clone.Translate(origin);
		serializer.AddContour(clone);
	}
}

Box MacroThermal::GetBox() const {
	Box box;
	for (const Contour &c : m_contours) {
		for (std::shared_ptr<Segment> s : c.GetSegments()) {
			box = box.Extend(s->GetBox());
		}
	}
	return box;
}

bool MacroThermal::operator ==(const MacroThermal &rhs) const {
	return m_contours == rhs.m_contours;
}

bool MacroThermal::operator !=(const MacroThermal &rhs) const {
	return m_contours != rhs.m_contours;
}

void MacroThermal::ApplyTransform(const gerbex::Transform &transform) {
	for (Contour &c : m_contours) {
		c.Transform(transform);
	}
}

const std::array<Contour, 4>& MacroThermal::GetContours() const {
	return m_contours;
}

} /* namespace gerbex */
