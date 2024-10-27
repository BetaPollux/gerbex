/*
 * RegionContour.cpp
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

#include <Contour.h>
#include "Point.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

Contour::Contour() {
	// Empty

}

bool Contour::IsClosed() const {
	//Checks that all segments are connected.
	//Does NOT check for more complex conditions which are invalid.
	if (m_segments.size() > 2) {
		bool closedEnd = (m_segments.front()->GetStart()
				== m_segments.back()->GetEnd());
		bool connected = true;
		for (size_t i = 1; i < m_segments.size(); i++) {
			connected &= (m_segments[i]->GetStart()
					== m_segments[i - 1]->GetEnd());
		}
		return closedEnd && connected;
	} else {
		return false;
	}
}

void Contour::AddSegment(const std::shared_ptr<Segment> &segment) {
	//TODO what about a full circle arc?
	if (segment->GetStart() == segment->GetEnd()) {
		throw std::invalid_argument("contour cannot have zero-length segment");
	}
	m_segments.push_back(segment);
}

const std::vector<std::shared_ptr<Segment>>& Contour::GetSegments() const {
	return m_segments;
}

void Contour::Translate(const Point &offset) {
	for (std::shared_ptr<Segment> s : m_segments) {
		s->Translate(offset);
	}
}

void Contour::Transform(const gerbex::Transform &transform) {
	for (std::shared_ptr<Segment> s : m_segments) {
		s->Transform(transform);
	}
}

} /* namespace gerbex */
