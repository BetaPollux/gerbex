/*
 * MacroVectorLine.cpp
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

#include "MacroVectorLine.h"
#include <stdexcept>

namespace gerbex {

MacroVectorLine::MacroVectorLine() :
		MacroVectorLine(MacroExposure::ON, 1.0, Point(0.0, 0.0),
				Point(1.0, 0.0), 0.0) {
	// Empty
}

MacroVectorLine::MacroVectorLine(MacroExposure exposure, double width,
		const Point &start, const Point &end, double rotation) :
		MacroPrimitive(exposure, start, rotation), m_width { width }, m_end {
				end } {
	if (width < 0.0) {
		throw std::invalid_argument("Width must be >= 0.0");
	}
}

MacroVectorLine::~MacroVectorLine() {
	// Empty
}

const Point& MacroVectorLine::GetEnd() const {
	return m_end;
}

double MacroVectorLine::GetWidth() const {
	return m_width;
}

std::unique_ptr<MacroVectorLine> MacroVectorLine::FromParameters(
		const Parameters &params) {
	if (params.size() != 7) {
		throw std::invalid_argument("macro vector line expects 7 parameters");
	}
	MacroExposure exposure = MacroPrimitive::ExposureFromNum((int) params[0]);
	double width = params[1];
	Point start(params[2], params[3]);
	Point end(params[4], params[5]);
	double rotation = params[6];
	return std::make_unique<MacroVectorLine>(exposure, width, start, end,
			rotation);
}

} /* namespace gerbex */
