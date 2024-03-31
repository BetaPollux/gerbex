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
#include <stdexcept>

Polygon::Polygon()
	: Polygon(1.0, 3)
{
	// Empty
}

Polygon::Polygon(double outerDiameter, int numVertices, double rotation,
		double holeDiameter)
	: m_outerDiameter{ outerDiameter },
	  m_numVertices{ numVertices },
	  m_rotation{ rotation },
	  m_holeDiameter{ holeDiameter }
{
	if (m_outerDiameter <= 0.0) {
		throw std::invalid_argument("Diameter must be > 0");
	}

	if (m_numVertices < 3 || m_numVertices > 12) {
		throw std::invalid_argument("Vertices must be from 3 to 12");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("Hole diameter must be >= 0");
	}
}

Polygon::~Polygon() {
	// Empty
}

double Polygon::GetHoleDiameter() const {
	return m_holeDiameter;
}

int Polygon::GetNumVertices() const {
	return m_numVertices;
}

double Polygon::GetOuterDiameter() const {
	return m_outerDiameter;
}

double Polygon::GetRotation() const {
	return m_rotation;
}
