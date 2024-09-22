/*
 * MacroCircle.h
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

#ifndef MACROCIRCLE_H_
#define MACROCIRCLE_H_

#include "MacroPrimitive.h"

namespace gerbex {

/*
 * Circle primitive defined by its center point and diameter.
 */
class MacroCircle: public MacroPrimitive {
public:
	MacroCircle();
	MacroCircle(MacroExposure exposure, double diameter,
			const RealPoint &center, double rotation = 0.0);
	virtual ~MacroCircle();
	double GetDiameter() const;

private:
	double m_diameter;
};

} /* namespace gerbex */

#endif /* MACROCIRCLE_H_ */
