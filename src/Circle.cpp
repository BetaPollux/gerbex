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
#include <stdexcept>

Circle::Circle()
	: m_diameter{ 1.0 },
	  m_holeDiameter{ 0.0 }
{
	// Empty
}

Circle::Circle(double diameter, double holeDiameter)
	: m_diameter{ diameter },
	  m_holeDiameter{ holeDiameter }
{
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

std::unique_ptr<Aperture> Circle::DeriveFrom(int numParameters, double *parameters) {
	if (numParameters == 1) {
		return std::make_unique<Circle>(parameters[0]);
	} else if (numParameters == 2) {
		return std::make_unique<Circle>(parameters[0], parameters[1]);
	}
	else {
		throw std::invalid_argument("Requires 1 or 2 parameters");
	}
}

