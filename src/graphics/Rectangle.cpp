/*
 * Rectangle.cpp
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

#include "Rectangle.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

Rectangle::Rectangle() :
		Rectangle { 1.0, 1.0 } {
	// Empty
}

Rectangle::Rectangle(double xSize, double ySize, double holeDiameter) :
		m_holeDiameter { holeDiameter } {
	if (xSize <= 0.0 || ySize <= 0.0) {
		throw std::invalid_argument("size must be > 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("hole diameter must be >= 0");
	}

	double dx = 0.5 * xSize;
	double dy = 0.5 * ySize;
	m_vertices = { { dx, dy }, { -dx, dy }, { -dx, -dy }, { dx, -dy } };
}

double Rectangle::GetHoleDiameter() const {
	return m_holeDiameter;
}

void Rectangle::Serialize(Serializer &serializer, const Point &origin) const {
	std::vector<Point> vertices = m_vertices;
	for (Point &p : vertices) {
		p += origin;
	}
	serializer.AddPolygon(vertices, m_polarity);
}

Box Rectangle::GetBox() const {
	return Box(m_vertices);
}

std::unique_ptr<Aperture> Rectangle::Clone() const {
	return std::make_unique<Rectangle>(*this);
}

void Rectangle::ApplyTransform(const Transform &transform) {
	m_holeDiameter = transform.ApplyScaling(m_holeDiameter);
	for (Point &p : m_vertices) {
		p = transform.Apply(p);
	}
}

const std::vector<Point>& Rectangle::GetVertices() const {
	return m_vertices;
}

} /* namespace gerbex */

