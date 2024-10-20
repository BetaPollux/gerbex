/*
 * Circle.cpp
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

#include "Circle.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

Circle::Circle() :
		Circle { 1.0 } {
	// Empty
}

Circle::Circle(double diameter, double holeDiameter) :
		m_diameter { diameter }, m_holeDiameter { holeDiameter } {
	if (diameter < 0.0) {
		throw std::invalid_argument("Diameter must be >= 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("Hole diameter must be >= 0");
	}
}

Circle::~Circle() {
	// Empty
}

double Circle::GetDiameter() const {
	return m_diameter;
}

double Circle::GetHoleDiameter() const {
	return m_holeDiameter;
}

void Circle::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	double radius = 0.5 * transform.ApplyScaling(m_diameter);
	serializer.AddCircle(radius, origin,
			transform.GetPolarity() == Polarity::Dark);
}

Box Circle::GetBox() const {
	return Box(m_diameter, Point());
}

} /* namespace gerbex */

