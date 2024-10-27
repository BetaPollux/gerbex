/*
 * MacroOutline.cpp
 *
 *  Created on: Apr. 1, 2024
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

#include "MacroOutline.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

MacroOutline::MacroOutline() :
		MacroOutline(MacroExposure::ON, { Point(0.0, 0.0), Point(1.0, 0.0),
				Point(0.0, 1.0) }, 0.0) {
	// Empty
}

MacroOutline::MacroOutline(MacroExposure exposure,
		const std::vector<Point> &vertices, double rotation) :
		MacroPrimitive(exposure), m_vertices { vertices } {
	if (vertices.size() < 3) {
		throw std::invalid_argument("There must at least 3 vertices");
	}
	for (Point &p : m_vertices) {
		p.Rotate(rotation);
	}
}

const std::vector<Point>& MacroOutline::GetVertices() const {
	return m_vertices;
}

std::unique_ptr<MacroOutline> MacroOutline::FromParameters(
		const Parameters &params) {
	if (params.size() < 5) {
		throw std::invalid_argument(
				"macro outline expects at least 5 parameters");
	}
	MacroExposure exposure = MacroPrimitive::ExposureFromNum((int) params[0]);
	size_t num = (size_t) params[1] + 1;

	if (params.size() != (5 + 2 * (num - 1))) {
		throw std::invalid_argument("macro outline expects 5+2n parameters");
	}
	std::vector<Point> vertices(num);
	auto it = params.begin() + 2;
	for (size_t i = 0; i < num; i++) {
		double x = *it++;
		double y = *it++;
		vertices[i] = Point(x, y);
	}
	double rotation = params.back();
	return std::make_unique<MacroOutline>(exposure, vertices, rotation);
}

void MacroOutline::Serialize(Serializer &serializer,
		const Point &origin) const {
	std::vector<Point> vertices = m_vertices;
	for (Point &p : vertices) {
		p += origin;
	}
	serializer.AddPolygon(vertices);
}

Box MacroOutline::GetBox() const {
	return Box(m_vertices);
}

bool MacroOutline::operator ==(const MacroOutline &rhs) const {
	return m_exposure == rhs.m_exposure && m_vertices == rhs.m_vertices;
}

bool MacroOutline::operator !=(const MacroOutline &rhs) const {
	return m_exposure != rhs.m_exposure || m_vertices != rhs.m_vertices;
}

void MacroOutline::ApplyTransform(const gerbex::Transform &transform) {
	for (Point &p : m_vertices) {
		p.ApplyTransform(transform);
	}
}

} /* namespace gerbex */

