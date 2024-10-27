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
#include "Contour.h"
#include "MacroPrimitive.h"
#include <array>
#include <memory>

namespace gerbex {

/*
 * A thermal pad primitive, being a ring interrupted by four gaps.
 */
class MacroThermal: public MacroPrimitive {
public:
	MacroThermal();
	MacroThermal(const Point &center, double outerDiameter,
			double innerDiameter, double gapThickness, double rotation);
	virtual ~MacroThermal() = default;
	bool operator==(const MacroThermal &rhs) const;
	bool operator!=(const MacroThermal &rhs) const;
	static std::unique_ptr<MacroThermal> FromParameters(
			const Parameters &params);
	void Serialize(Serializer &serializer, const Point &origin) const override;
	Box GetBox() const override;
	void ApplyTransform(const Transform &transform) override;
	const std::array<Contour, 4>& GetContours() const;

private:
	std::array<Contour, 4> m_contours;

};

} /* namespace gerbex */

#endif /* MACROTHERMAL_H_ */
