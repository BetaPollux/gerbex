/*
 * Obround.cpp
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

#include "Obround.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

Obround::Obround() :
		Obround { 1.0, 1.0 } {
	// Empty
}

Obround::Obround(double xSize, double ySize, double holeDiameter) {
	if (xSize <= 0.0 || ySize <= 0.0) {
		throw std::invalid_argument("size must be > 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("hole diameter must be >= 0");
	}
	if (xSize > ySize) {
		m_drawWidth = ySize;
		double dx = 0.5 * (xSize - m_drawWidth);
		m_segment = Segment(Point(-dx, 0.0), Point(dx, 0.0));
	} else {
		m_drawWidth = xSize;
		double dy = 0.5 * (ySize - m_drawWidth);
		m_segment = Segment(Point(0.0, -dy), Point(0.0, dy));
	}

	m_holeDiameter = holeDiameter;
}

double Obround::GetHoleDiameter() const {
	return m_holeDiameter;
}

void Obround::Serialize(Serializer &serializer, const Point &origin) const {
	Segment segment = m_segment;
	segment.Translate(origin);
	serializer.AddDraw(m_drawWidth, segment, m_polarity);
}

Box Obround::GetBox() const {
	double radius = 0.5 * m_drawWidth;
	return m_segment.GetBox().Pad(radius);
}

std::unique_ptr<Aperture> Obround::Clone() const {
	return std::make_unique<Obround>(*this);
}

double Obround::GetLength() const {
	double distance = m_segment.GetEnd().Distance(m_segment.GetStart());
	return distance + m_drawWidth;
}

double Obround::GetDrawWidth() const {
	return m_drawWidth;
}

const Segment& Obround::GetSegment() const {
	return m_segment;
}

bool Obround::operator ==(const Obround &rhs) const {
	return m_segment == rhs.m_segment && m_drawWidth == rhs.m_drawWidth
			&& m_holeDiameter == rhs.m_holeDiameter
			&& m_polarity == rhs.m_polarity;
}

bool Obround::operator !=(const Obround &rhs) const {
	return m_segment != rhs.m_segment || m_drawWidth != rhs.m_drawWidth
				|| m_holeDiameter != rhs.m_holeDiameter
				|| m_polarity != rhs.m_polarity;
}

void Obround::ApplyTransform(const Transform &transform) {
	m_polarity = transform.ApplyPolarity(m_polarity);
	m_holeDiameter = transform.ApplyScaling(m_holeDiameter);
	m_segment.Transform(transform);
	m_drawWidth = transform.ApplyScaling(m_drawWidth);
}

} /* namespace gerbex */
