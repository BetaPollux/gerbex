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

namespace gerbex {

Segment::Segment() {
	// Empty

}

Segment::Segment(const Point &origin, const Point &endPoint)
{
	m_origin = origin;
	m_endPoint = endPoint;
}

Segment::Segment(const Point &origin, const Point &endPoint,
		std::shared_ptr<Aperture> aperture,
		const ApertureTransformation &transformation)
	: GraphicalObject(origin, aperture, transformation),
	  m_endPoint{ endPoint }
{
	// Empty
}

Segment::~Segment() {
	// Empty
}

const Point& Segment::GetEndPoint() const {
	return m_endPoint;
}

} /* namespace gerbex */
