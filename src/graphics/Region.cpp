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

Region::Region() {
	// Empty
}

Region::Region(Polarity polarity) {
	m_polarity = polarity;
}

void Region::StartContour() {
	if (!m_contours.empty() && !m_contours.back().IsClosed()) {
		throw std::logic_error("need to close contour before starting next");
	}
	m_contours.push_back(Contour());
}

void Region::AddSegment(const std::shared_ptr<Segment> &segment) {
	if (m_contours.empty()) {
		throw std::logic_error("need to start a contour before adding segment");
	}
	m_contours.back().AddSegment(segment);
}

const std::vector<Contour>& Region::GetContours() const {
	return m_contours;
}

bool Region::AreContoursClosed() const {
	for (const Contour &c : m_contours) {
		if (!c.IsClosed()) {
			return false;
		}
	}
	return !m_contours.empty();
}

void Region::Serialize(Serializer &serializer, const Point &origin) const {
	pSerialItem dest = serializer.GetTarget(m_polarity);
	for (const Contour &c : m_contours) {
		Contour clone = c;
		clone.Translate(origin);
		serializer.AddContour(dest, clone);
	}
}

Box Region::GetBox() const {
	if (!AreContoursClosed()) {
		throw std::invalid_argument("cannot get box for open contours");
	}
	Box box = m_contours.front().GetSegments().front()->GetBox();
	for (const Contour &c : m_contours) {
		for (std::shared_ptr<Segment> s : c.GetSegments()) {
			box = box.Extend(s->GetBox());
		}
	}
	return box;
}

void Region::Translate(const Point &offset) {
	for (Contour &c : m_contours) {
		c.Translate(offset);
	}
}

void Region::ApplyTransform(const gerbex::Transform &transform) {
	for (Contour &c : m_contours) {
		c.Transform(transform);
	}
}

std::unique_ptr<GraphicalObject> Region::Clone() {
	return std::make_unique<Region>(*this);
}

} /* namespace gerbex */
