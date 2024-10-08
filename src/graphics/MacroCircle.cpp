/*
 * MacroCircle.cpp
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

#include "MacroCircle.h"
#include <stdexcept>

namespace gerbex {

MacroCircle::MacroCircle() :
		MacroCircle(MacroExposure::ON, 1.0, RealPoint(0.0, 0.0), 0.0) {
	// Empty
}

MacroCircle::MacroCircle(MacroExposure exposure, double diameter,
		const RealPoint &center, double rotation) :
		MacroPrimitive(exposure, center, rotation), m_diameter { diameter } {
	if (diameter < 0.0) {
		throw std::invalid_argument("Diameter must be >= 0.0");
	}
}

MacroCircle::~MacroCircle() {
	// Empty
}

double MacroCircle::GetDiameter() const {
	return m_diameter;
}

std::unique_ptr<MacroCircle> MacroCircle::FromParameters(
		const Parameters &params) {
	if (params.size() < 4 || params.size() > 5) {
		throw std::invalid_argument("macro circle expects 4 to 5 parameters");
	}

	MacroExposure exposure = MacroPrimitive::ExposureFromNum((int) params[0]);
	double diameter = params[1];
	RealPoint center(params[2], params[3]);
	double rotation;
	if (params.size() > 4) {
		rotation = params[4];
	} else {
		rotation = 0.0;
	}
	return std::make_unique<MacroCircle>(exposure, diameter, center, rotation);
}

} /* namespace gerbex */
