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
#include <stdexcept>

namespace gerbex {

Region::Region() {
	// Empty

}

Region::Region(ApertureTransformation transformation) :
		GraphicalObject(Point(), nullptr, transformation) {
}

Region::~Region() {
	// Empty
}

void Region::StartContour() {
	if (!m_contours.empty() && !m_contours.back().IsClosed()) {
		throw std::logic_error("need to close contour before starting next");
	}
	m_contours.push_back(RegionContour());
}

void Region::AddSegment(std::shared_ptr<Segment> segment) {
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

} /* namespace gerbex */
