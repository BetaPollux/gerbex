/*
 * MacroOutline.h
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

#ifndef MACROOUTLINE_H_
#define MACROOUTLINE_H_

#include "MacroPrimitive.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * A polygon primitive defined by its outline or contour. It must be closed.
 */
class MacroOutline: public MacroPrimitive {
public:
	MacroOutline();
	MacroOutline(MacroExposure exposure, const std::vector<RealPoint> &vertices, double rotation);
	virtual ~MacroOutline();
	const std::vector<RealPoint>& GetVertices() const;

private:
	std::vector<RealPoint> m_vertices;
};

} /* namespace gerbex */

#endif /* MACROOUTLINE_H_ */