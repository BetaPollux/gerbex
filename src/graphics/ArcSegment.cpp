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

} /* namespace gerbex */
