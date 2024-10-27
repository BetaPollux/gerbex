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

#include "DataTypeParser.h"
#include "MacroPrimitive.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * Rectangle primitive defined by its width, height and center point.
 */
class MacroCenterLine: public MacroPrimitive {
public:
	MacroCenterLine();
	MacroCenterLine(MacroExposure exposure, double width, double height,
			const Point &center, double rotation);
	virtual ~MacroCenterLine() = default;
	double GetHeight() const;
	double GetWidth() const;
	static std::unique_ptr<MacroCenterLine> FromParameters(const Parameters &params);
	void Serialize(Serializer &serializer, const Point &origin) const override;
	const Point& GetCenter() const;
	Box GetBox() const override;
	void ApplyTransform(const Transform &transform) override;

private:
	std::vector<Point> getVertices() const;
	Point m_center;
	double m_width, m_height;
};

} /* namespace gerbex */

#endif /* MACROCENTERLINE_H_ */
