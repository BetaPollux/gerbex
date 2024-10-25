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
		throw std::invalid_argument("Size must be > 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("Hole diameter must be >= 0");
	}
	if (xSize > ySize) {
		m_radius = 0.5 * ySize;
		double dx = 0.5 * xSize - m_radius;
		m_segment = Segment(Point(-dx, 0.0), Point(dx, 0.0));
	} else {
		m_radius = 0.5 * xSize;
		double dy = 0.5 * ySize - m_radius;
		m_segment = Segment(Point(0.0, -dy), Point(0.0, dy));
	}

	m_holeDiameter = holeDiameter;
}

double Obround::GetHoleDiameter() const {
	return m_transform.ApplyScaling(m_holeDiameter);
}

double Obround::GetXSize() const {
	Point delta = m_segment.GetEnd() - m_segment.GetStart();
	return m_transform.ApplyScaling(2.0 * m_radius + fabs(delta.GetX()));
}

double Obround::GetYSize() const {
	Point delta = m_segment.GetEnd() - m_segment.GetStart();
	return m_transform.ApplyScaling(2.0 * m_radius + fabs(delta.GetY()));
}

void Obround::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	double width = transform.ApplyScaling(2.0 * m_radius);
	Segment segment = m_segment;
	segment.Transform(transform);
	segment.Translate(origin);
	serializer.AddDraw(width, segment);
}

Box Obround::GetBox() const {
	double radius = m_transform.ApplyScaling(m_radius);
	Segment segment = m_segment;
	segment.Transform(m_transform);
	return segment.GetBox().Pad(radius);
}

std::unique_ptr<Aperture> Obround::Clone() const {
	return std::make_unique<Obround>(*this);
}

} /* namespace gerbex */
