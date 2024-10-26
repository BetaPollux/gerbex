/*
 * Segment.cpp
 *
 *  Created on: Mar. 29, 2024
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

#include "Segment.h"
#include <algorithm>

namespace gerbex {

Segment::Segment() :
		m_start { 0.0, 0.0 }, m_end { 1.0, 0.0 } {
	// Empty

}

Segment::Segment(const Point &start, const Point &end) {
	m_start = start;
	m_end = end;
}

const Point& Segment::GetStart() const {
	return m_start;
}

const Point& Segment::GetEnd() const {
	return m_end;
}

Box Segment::GetBox() const {
	Point delta = m_end - m_start;
	double left = std::min(m_start.GetX(), m_end.GetX());
	double bottom = std::min(m_start.GetY(), m_end.GetY());
	return Box(fabs(delta.GetX()), fabs(delta.GetY()), left, bottom);
}

void Segment::Translate(const Point &offset) {
	m_start += offset;
	m_end += offset;
}

void Segment::Transform(const gerbex::Transform &transform) {
	m_start = transform.Apply(m_start);
	m_end = transform.Apply(m_end);
}

} /* namespace gerbex */
