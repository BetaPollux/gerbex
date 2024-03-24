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
#include <stdexcept>

Obround::Obround()
	: m_xSize{ 1.0 },
	  m_ySize{ 1.0 },
	  m_holeDiameter{ 0.0 }
{
	// Empty
}

Obround::Obround(double xSize, double ySize, double holeDiameter)
	: m_xSize{ xSize },
	  m_ySize{ ySize },
	  m_holeDiameter{ holeDiameter }
{
	if (xSize <= 0.0 || ySize <= 0.0) {
		throw std::invalid_argument("Size must be > 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("Hole diameter must be >= 0");
	}
}

Obround::~Obround() {
	// Empty
}

std::unique_ptr<Aperture> Obround::DeriveFrom(int numParameters, double *parameters) {
	if (numParameters == 2) {
		return std::make_unique<Obround>(parameters[0], parameters[1]);
	} else if (numParameters == 3) {
		return std::make_unique<Obround>(parameters[0], parameters[1], parameters[2]);
	}
	else {
		throw std::invalid_argument("Requires 2 or 3 parameters");
	}
}
