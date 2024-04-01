/*
 * MacroPolygon.cpp
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

#include "MacroPolygon.h"
#include <stdexcept>

MacroPolygon::MacroPolygon()
	: MacroPolygon(MacroExposure::ON, 6, RealPoint(0.0, 0.0), 1.0, 0.0)
{
	// Empty
}

MacroPolygon::MacroPolygon(MacroExposure exposure, int numVertices,
		const RealPoint &center, double diameter, double rotation)
	: MacroPrimitive(exposure, center, rotation),
	  m_numVertices{ numVertices },
	  m_diameter{ diameter }
{
	if (numVertices < 3 || numVertices > 12) {
		throw std::invalid_argument("Number of vertices must be from 3 to 12");
	}

	if (diameter < 0.0) {
		throw std::invalid_argument("Diameter must be >= 0.0");
	}
}

MacroPolygon::~MacroPolygon() {
	// Empty
}

double MacroPolygon::GetDiameter() const {
	return m_diameter;
}

int MacroPolygon::GetNumVertices() const {
	return m_numVertices;
}
