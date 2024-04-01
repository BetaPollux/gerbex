/*
 * MacroPrimitive.h
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

#ifndef MACROPRIMITIVE_H_
#define MACROPRIMITIVE_H_

#include "Point.h"

enum class MacroExposure {
	OFF,
	ON
};

/*
 * A simple shape used to build a Macro aperture.
 */
class MacroPrimitive {
public:
	MacroPrimitive();
	MacroPrimitive(MacroExposure exposure, RealPoint coord, double rotation);
	virtual ~MacroPrimitive();

private:
	MacroExposure m_exposure;
	RealPoint m_coord;
	double m_rotation;
};

#endif /* MACROPRIMITIVE_H_ */
