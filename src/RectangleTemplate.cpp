/*
 * RectangleTemplate.cpp
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

#include "RectangleTemplate.h"
#include <stdexcept>

RectangleTemplate::RectangleTemplate() {
	// Empty

}

RectangleTemplate::~RectangleTemplate() {
	// Empty
}

std::unique_ptr<Aperture> RectangleTemplate::Call(
		const std::vector<double> &parameters) {
	if (parameters.size() == 2) {
		return std::make_unique<Rectangle>(parameters[0], parameters[1]);
	} else if (parameters.size() == 3) {
		return std::make_unique<Rectangle>(parameters[0], parameters[1], parameters[2]);
	}
	else {
		throw std::invalid_argument("Requires 2 or 3 parameters");
	}
}

