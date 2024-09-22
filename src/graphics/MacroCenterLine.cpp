/*
 * MacroCenterLine.cpp
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

#include "MacroCenterLine.h"
#include <stdexcept>

namespace gerbex {

MacroCenterLine::MacroCenterLine()
	: MacroCenterLine(MacroExposure::ON, 1.0, 0.5, RealPoint(0.0, 0.0), 0.0)
{
	// Empty
}

MacroCenterLine::MacroCenterLine(MacroExposure exposure, double width,
		double height, const RealPoint &start, double rotation)
	: MacroPrimitive(exposure, start, rotation),
	  m_width{ width},
	  m_height{ height }
{
	if (width < 0.0 || height < 0.0) {
		throw std::invalid_argument("Width and height must be >= 0.0");
	}
}

MacroCenterLine::~MacroCenterLine() {
	// Empty
}

double MacroCenterLine::GetHeight() const {
	return m_height;
}

double MacroCenterLine::GetWidth() const {
	return m_width;
}

} /* namespace gerbex */
