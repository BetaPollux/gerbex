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
	: Obround{ 1.0, 1.0 }
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

double Obround::GetHoleDiameter() const {
	return m_holeDiameter;
}

double Obround::GetXSize() const {
	return m_xSize;
}

double Obround::GetYSize() const {
	return m_ySize;
}
