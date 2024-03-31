/*
 * PolygonTemplate.cpp
 *
 *  Created on: Mar. 29, 2024
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

#include "PolygonTemplate.h"
#include <stdexcept>

PolygonTemplate::PolygonTemplate() {
	// Empty

}

PolygonTemplate::~PolygonTemplate() {
	// Empty
}

std::unique_ptr<Aperture> PolygonTemplate::Call(int numParameters, double *parameters) {
	if (numParameters == 2) {
			return std::make_unique<Polygon>(parameters[0], (int)parameters[1]);
	} else if (numParameters == 3) {
		return std::make_unique<Polygon>(parameters[0], (int)parameters[1], parameters[2]);
	} else if (numParameters == 4) {
		return std::make_unique<Polygon>(parameters[0], (int)parameters[1], parameters[2], parameters[3]);
	}
	else {
		throw std::invalid_argument("Requires 2 to 4 parameters");
	}
}
