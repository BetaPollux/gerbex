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
		m_xSize { xSize }, m_ySize { ySize }, m_holeDiameter { holeDiameter } {
	if (xSize <= 0.0 || ySize <= 0.0) {
		throw std::invalid_argument("Size must be > 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("Hole diameter must be >= 0");
	}
}

double Rectangle::GetHoleDiameter() const {
	return m_transform.ApplyScaling(m_holeDiameter);
}

double Rectangle::GetXSize() const {
	return m_transform.ApplyScaling(m_xSize);
}

double Rectangle::GetYSize() const {
	return m_transform.ApplyScaling(m_ySize);
}

void Rectangle::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	std::vector<Point> vertices = transform.ApplyThenTranslate(getVertices(),
			origin);
	serializer.AddPolygon(vertices);
}

std::vector<Point> Rectangle::getVertices() const {
	// Rectangle is centered on origin
	double dx = 0.5 * m_xSize;
	double dy = 0.5 * m_ySize;
	std::vector<Point> vertices = { { dx, dy }, { -dx, dy }, { -dx, -dy }, { dx,
			-dy } };
	for (Point &p : vertices) {
		p = m_transform.Apply(p);
	}
	return vertices;
}

Box Rectangle::GetBox() const {
	return Box(getVertices());
}

std::unique_ptr<Aperture> Rectangle::Clone() const {
	std::unique_ptr<Rectangle> clone = std::make_unique<Rectangle>(m_xSize,
			m_ySize, m_holeDiameter);
	clone->SetTransform(m_transform);
	return clone;
}

} /* namespace gerbex */

