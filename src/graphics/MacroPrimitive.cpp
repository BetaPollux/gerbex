/*
 * MacroPrimitive.cpp
 *
 *  Created on: Mar. 30, 2024
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

#include "MacroPrimitive.h"
#include <stdexcept>

namespace gerbex {

MacroPrimitive::MacroPrimitive()
	: MacroPrimitive(MacroExposure::ON, Point(0.0, 0.0), 0.0)
{
	// Empty
}

MacroPrimitive::MacroPrimitive(MacroExposure exposure, const Point &coord, double rotation)
	: m_exposure{ exposure },
	  m_coord{ coord },
	  m_rotation{ rotation }
{
	// Empty
}

MacroPrimitive::~MacroPrimitive() {
	// Empty
}

const Point& MacroPrimitive::GetCoord() const {
	return m_coord;
}

MacroExposure MacroPrimitive::GetExposure() const {
	return m_exposure;
}

double MacroPrimitive::GetRotation() const {
	return m_rotation;
}

MacroExposure MacroPrimitive::ExposureFromNum(int num) {
	switch (num) {
	case 1:
		return MacroExposure::ON;
	case 0:
		return MacroExposure::OFF;
	default:
		throw std::invalid_argument("macro exposure must be 0 or 1");
	}
}

} /* namespace gerbex */
