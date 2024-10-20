/*
 * MacroVectorLine.h
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

#ifndef MACROVECTORLINE_H_
#define MACROVECTORLINE_H_

#include "DataTypeParser.h"
#include "MacroPrimitive.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * A rectangle primitive defined by its line width, start and end points.
 */
class MacroVectorLine: public MacroPrimitive {
public:
	MacroVectorLine();
	MacroVectorLine(MacroExposure exposure, double width,
			const Point &start, const Point &end, double rotation);
	virtual ~MacroVectorLine() = default;
	const Point& GetStart() const;
	const Point& GetEnd() const;
	double GetWidth() const;
	static std::unique_ptr<MacroVectorLine> FromParameters(const Parameters &params);
	void Serialize(Serializer &serializer, const Point &origin,
			const ApertureTransformation &transform) const override;
	Box GetBox() const override;

private:
	std::vector<Point> getCorners() const;
	Point m_start, m_end;
	double m_width;
};

} /* namespace gerbex */

#endif /* MACROVECTORLINE_H_ */
