/*
 * MacroThermal.cpp
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

#include "MacroThermal.h"
#include <stdexcept>

MacroThermal::MacroThermal()
	: MacroThermal(RealPoint(0.0, 0.0), 1.0, 0.5, 0.25, 0.0)
{
	// Empty
}

MacroThermal::MacroThermal(const RealPoint &center, double outerDiameter,
		double innerDiameter, double gapThickness, double rotation)
	: MacroPrimitive(MacroExposure::ON, center, rotation),
	  m_outerDiameter{ outerDiameter },
	  m_innerDiameter{ innerDiameter },
	  m_gapThickness{ gapThickness }
{
	if (outerDiameter < 0.0 || innerDiameter < 0.0 || gapThickness < 0.0) {
		throw std::invalid_argument("Diameters and gap must be >= 0.0");
	}
	if (outerDiameter <= innerDiameter) {
		throw std::invalid_argument("Outer diameter must be larger than inner diameter");
	}
	if (gapThickness > outerDiameter / 1.4142) {
		throw std::invalid_argument("Gap thickness must be less than (outer diameter)/sqrt(2)");
	}
}

MacroThermal::~MacroThermal() {
	// Empty
}

double MacroThermal::GetGapThickness() const {
	return m_gapThickness;
}

double MacroThermal::GetInnerDiameter() const {
	return m_innerDiameter;
}

double MacroThermal::GetOuterDiameter() const {
	return m_outerDiameter;
}