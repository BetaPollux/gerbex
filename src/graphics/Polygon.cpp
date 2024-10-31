/*
 * Polygon.cpp
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

#include "Polygon.h"
#include "Serializer.h"
#include <cmath>
#include <stdexcept>

namespace gerbex {

Polygon::Polygon() :
		Polygon(1.0, 3) {
	// Empty
}

Polygon::Polygon(double outerDiameter, int numVertices, double rotation,
		double holeDiameter) :
		m_holeDiameter { holeDiameter } {
	if (outerDiameter <= 0.0) {
		throw std::invalid_argument("diameter must be > 0");
	}

	if (numVertices < 3 || numVertices > 12) {
		throw std::invalid_argument("vertices must be from 3 to 12");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("hole diameter must be >= 0");
	}

	m_vertices.reserve(numVertices);
	double angle_step = 2.0 * M_PI / numVertices;
	double angle0 = M_PI / 180.0 * rotation;
	double radius = 0.5 * outerDiameter;
	for (int i = 0; i < numVertices; i++) {
		double angle = angle_step * i + angle0;
		double x = radius * cos(angle);
		double y = radius * sin(angle);
		m_vertices.push_back(Point(x, y));
	}
}

double Polygon::GetHoleDiameter() const {
	return m_holeDiameter;
}

void Polygon::Serialize(Serializer &serializer, pSerialItem target,
		const Point &origin) const {
	std::vector<Point> vertices = m_vertices;
	for (Point &p : vertices) {
		p += origin;
	}
	pSerialItem poly = serializer.AddPolygon(target, vertices);
	if (m_holeDiameter > 0) {
		pSerialItem mask = serializer.NewMask(GetBox().Translate(origin));
		serializer.AddCircle(mask, 0.5 * m_holeDiameter, origin);
		serializer.SetMask(poly, mask);
	}
}

Box Polygon::GetBox() const {
	return Box(m_vertices);
}

std::unique_ptr<Aperture> Polygon::Clone() const {
	return std::make_unique<Polygon>(*this);
}

void Polygon::ApplyTransform(const Transform &transform) {
	m_holeDiameter *= transform.GetScaling();
	for (Point &p : m_vertices) {
		p.ApplyTransform(transform);
	}
}

bool Polygon::operator ==(const Polygon &rhs) const {
	return m_vertices == rhs.m_vertices && m_holeDiameter == rhs.m_holeDiameter;
}

bool Polygon::operator !=(const Polygon &rhs) const {
	return m_vertices != rhs.m_vertices || m_holeDiameter != rhs.m_holeDiameter;
}

const std::vector<Point>& Polygon::GetVertices() const {
	return m_vertices;
}

} /* namespace gerbex */

