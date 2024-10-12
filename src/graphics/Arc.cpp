/*
 * Arc.cpp
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

namespace gerbex {

Arc::Arc() :
		m_direction { ArcDirection::Clockwise }, m_centerOffset { } {
	// Empty
}

Arc::Arc(const Point &origin, const Point &endPoint, const Point &centerOffset,
		ArcDirection direction) :
		Segment(origin, endPoint), m_direction { direction }, m_centerOffset {
				centerOffset } {
	// Empty
}

Arc::Arc(const Point &origin, const Point &endPoint, const Point &centerOffset,
		ArcDirection direction, std::shared_ptr<Aperture> aperture,
		const ApertureTransformation &transformation) :
		Segment(origin, endPoint, aperture, transformation), m_direction {
				direction }, m_centerOffset { centerOffset } {
	// Empty
}

Arc::~Arc() {
	// Empty
}

const Point& Arc::GetCenterOffset() const {
	return m_centerOffset;
}

ArcDirection Arc::GetDirection() const {
	return m_direction;
}

void Arc::Serialize(Serializer &serializer) {
	// TODO should not need to reset offset
	serializer.SetOffset(0.0, 0.0);
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(
			m_aperture);
	// TODO need to add Arc to serializer
	double cx = m_centerOffset.GetX() + m_origin.GetX();
	double cy = m_centerOffset.GetY() + m_origin.GetY();
	serializer.AddDraw(circle->GetDiameter(), cx, cy, m_origin.GetX(),
			m_origin.GetY());
	serializer.AddDraw(circle->GetDiameter(), cx, cy, m_endPoint.GetX(),
			m_endPoint.GetY());
}

} /* namespace gerbex */
