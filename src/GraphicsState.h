/*
 * GraphicsState.h
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

#ifndef GRAPHICSSTATE_H_
#define GRAPHICSSTATE_H_

#include "Aperture.h"
#include "ApertureTransformation.h"
#include "CoordinateFormat.h"
#include "Point.h"


enum class Unit {
	Invalid,
	Millimeter,
	Inch
};


enum class PlotState {
	Invalid,
	Linear,
	Clockwise,
	Counter_clockwise
};


/*
 * Graphics state is a set of parameters affecting the result of operation codes.
 */
class GraphicsState {
public:
	GraphicsState();
	virtual ~GraphicsState();

private:
	CoordinateFormat m_format;
	Unit m_unit;
	Point m_currentPoint;
	Aperture *m_currentAperture;
	PlotState m_plotState;
	ApertureTransformation m_transformation;
};

#endif /* GRAPHICSSTATE_H_ */
