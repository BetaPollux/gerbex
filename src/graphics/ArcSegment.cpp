/*
 * ArcSegment.cpp
 *
 *  Created on: Oct. 15, 2024
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
#include "ArcSegment.h"

namespace gerbex {

ArcSegment::ArcSegment() :
		Segment(), m_centerOffset { }, m_direction { ArcDirection::Clockwise } {
	// Empty
}

ArcSegment::ArcSegment(const Point &start, const Point &end,
		const Point &centerOffset, ArcDirection direction) :
		Segment(start, end), m_centerOffset { centerOffset }, m_direction {
				direction } {
	// Empty
}

const Point& ArcSegment::GetCenterOffset() const {
	return m_centerOffset;
}

Point ArcSegment::GetCenter() const {
	return m_start + m_centerOffset;
}

ArcDirection ArcSegment::GetDirection() const {
	return m_direction;
}

bool ArcSegment::IsCircle() const {
	return m_start == m_end;
}

double ArcSegment::GetRadius() const {
	return m_start.Distance(GetCenter());;
}

void ArcSegment::Transform(const ApertureTransformation &transform) {
	m_start = transform.Apply(m_start);
	m_end = transform.Apply(m_end);
	if (transform.GetMirroring() == Mirroring::X
			|| transform.GetMirroring() == Mirroring::Y) {
		if (m_direction == ArcDirection::CounterClockwise) {
			m_direction = ArcDirection::Clockwise;
		} else {
			m_direction = ArcDirection::CounterClockwise;
		}
	}
}

Box ArcSegment::GetBox() const {
	//TODO this is for a full circle, not the real arc
	Point c = GetCenter();
	double r = GetRadius();
	return Box(2.0 * r, 2.0 * r, c.GetX() - r, c.GetY() - r);
}

} /* namespace gerbex */
