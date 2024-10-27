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
		throw std::invalid_argument("diameter must be >= 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("hole diameter must be >= 0");
	}
}

double Circle::GetDiameter() const {
	return m_diameter;
}

double Circle::GetHoleDiameter() const {
	return m_holeDiameter;
}

void Circle::Serialize(Serializer &serializer, const Point &origin) const {
	double radius = 0.5 * m_diameter;
	serializer.AddCircle(radius, origin, m_polarity);
}

Box Circle::GetBox() const {
	return Box(GetDiameter(), Point());
}

bool Circle::operator ==(const Circle &rhs) const {
	return m_diameter == rhs.m_diameter && m_holeDiameter == rhs.m_holeDiameter
			&& m_polarity == rhs.m_polarity;
}

bool Circle::operator !=(const Circle &rhs) const {
	return m_diameter != rhs.m_diameter || m_holeDiameter != rhs.m_holeDiameter
			|| m_polarity != rhs.m_polarity;
}

std::unique_ptr<Aperture> Circle::Clone() const {
	return std::make_unique<Circle>(*this);
}

void Circle::ApplyTransform(const Transform &transform) {
	m_polarity = transform.ApplyPolarity(m_polarity);
	m_diameter = transform.ApplyScaling(m_diameter);
	m_holeDiameter = transform.ApplyScaling(m_holeDiameter);
}

} /* namespace gerbex */

