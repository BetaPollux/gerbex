/*
 * MacroPolygon.h
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

#ifndef MACROPOLYGON_H_
#define MACROPOLYGON_H_

#include "DataTypeParser.h"
#include "MacroPrimitive.h"
#include <memory>

namespace gerbex {

/*
 * A regular polygon primitive, which circumscribes a circle, defined by the number of vertices,
 * center point and its outer diameter.
 */
class MacroPolygon: public MacroPrimitive {
public:
	MacroPolygon();
	MacroPolygon(MacroExposure exposure, int numVertices,
			const RealPoint &center, double diameter, double rotation);
	virtual ~MacroPolygon();
	double GetDiameter() const;
	int GetNumVertices() const;
	static std::unique_ptr<MacroPolygon> FromParameters(const Parameters &params);

private:
	int m_numVertices;
	double m_diameter;
};

} /* namespace gerbex */

#endif /* MACROPOLYGON_H_ */
