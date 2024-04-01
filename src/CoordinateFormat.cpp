/*
 * CoordinateFormat.cpp
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

#include "CoordinateFormat.h"
#include <stdexcept>

CoordinateFormat::CoordinateFormat()
	: 	CoordinateFormat(3, 6)
{
	// Empty
}

CoordinateFormat::CoordinateFormat(int integer, int decimal)
	: 	m_integer{ integer },
		m_decimal{ decimal }
{
	if (integer < 1 || integer > 6) {
		throw std::invalid_argument("Integer must be from 1 to 6.");
	}

	switch (m_decimal) {
	case 5:
		m_resolution = 1e-5;
		break;
	case 6:
		m_resolution = 1e-6;
		break;
	default:
		throw std::invalid_argument("Decimal must be either 5 or 6.");
	}
}

CoordinateFormat::~CoordinateFormat() {
	// Empty
}

RealPoint CoordinateFormat::Convert(const Point &point) {
	return RealPoint(m_resolution * point.GetX(), m_resolution * point.GetY());
}

