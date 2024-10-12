/*
 * MacroThermal.h
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

#ifndef MACROTHERMAL_H_
#define MACROTHERMAL_H_

#include "DataTypeParser.h"
#include "MacroPrimitive.h"
#include <memory>

namespace gerbex {

/*
 * A thermal pad primitive, being a ring interrupted by four gaps.
 */
class MacroThermal: public MacroPrimitive {
public:
	MacroThermal();
	MacroThermal(const Point &center, double outerDiameter, double innerDiameter,
			double gapThickness, double rotation);
	virtual ~MacroThermal();
	double GetGapThickness() const;
	double GetInnerDiameter() const;
	double GetOuterDiameter() const;
	static std::unique_ptr<MacroThermal> FromParameters(const Parameters &params);

private:
	double m_outerDiameter, m_innerDiameter, m_gapThickness;
};

} /* namespace gerbex */

#endif /* MACROTHERMAL_H_ */
