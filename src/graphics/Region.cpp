/*
 * Region.cpp
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

#include "Region.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

Region::Region() :
		m_polarity { Polarity::Dark } {
	// Empty

}

Region::Region(Polarity polarity) :
		m_polarity { polarity } {
}

void Region::StartContour() {
	if (!m_contours.empty() && !m_contours.back().IsClosed()) {
		throw std::logic_error("need to close contour before starting next");
	}
	m_contours.push_back(RegionContour());
}

void Region::AddSegment(const std::shared_ptr<Segment> &segment) {
	if (m_contours.empty()) {
		throw std::logic_error("need to start a contour before adding segment");
	}
	m_contours.back().AddSegment(segment);
}

const std::vector<RegionContour>& Region::GetContours() const {
	return m_contours;
}

bool Region::AreContoursClosed() const {
	for (const RegionContour &c : m_contours) {
		if (!c.IsClosed()) {
			return false;
		}
	}
	return true;
}

void Region::Serialize(Serializer &serializer, const Point &origin,
		const ApertureTransformation &transform) {
	(void)origin;
	(void)transform;
	for (RegionContour &c : m_contours) {
		serializer.AddContour(c.GetSegments(), m_polarity == Polarity::Dark);
	}
}

Polarity Region::GetPolarity() const {
	return m_polarity;
}

Box Region::GetBox() const {
	if (!AreContoursClosed()) {
		throw std::invalid_argument("cannot get box for open contours");
	}
	Box box;
	for (const RegionContour &c : m_contours) {
		for (std::shared_ptr<Segment> s : c.GetSegments()) {
			box = box.Extend(s->GetBox());
		}
	}
	return box;
}

} /* namespace gerbex */
