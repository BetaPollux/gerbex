/*
 * MacroCenterLine.h
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

#ifndef MACROCENTERLINE_H_
#define MACROCENTERLINE_H_

#include "MacroPrimitive.h"

namespace gerbex {

/*
 * Rectangle primitive defined by its width, height and center point.
 */
class MacroCenterLine: public MacroPrimitive {
public:
	MacroCenterLine();
	MacroCenterLine(MacroExposure exposure, double width, double height,
			const RealPoint &start, double rotation);
	virtual ~MacroCenterLine();
	double GetHeight() const;
	double GetWidth() const;

private:
	double m_width, m_height;
};

} /* namespace gerbex */

#endif /* MACROCENTERLINE_H_ */