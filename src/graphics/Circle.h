/*
 * Circle.h
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

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include "Aperture.h"

namespace gerbex {

/*
 * A circle aperture with optional hole.
 * Diameter may be 0.
 */
class Circle: public Aperture {
public:
	Circle();
	Circle(double diameter, double holeDiameter = 0.0);
	virtual ~Circle() = default;
	bool operator==(const Circle &rhs) const;
	bool operator!=(const Circle &rhs) const;
	double GetDiameter() const;
	double GetHoleDiameter() const;
	void Serialize(Serializer &serializer, pSerialItem target, const Point &origin) const override;
	Box GetBox() const override;
	std::unique_ptr<Aperture> Clone() const override;
	void ApplyTransform(const Transform &transform) override;

private:
	double m_diameter;
	double m_holeDiameter;
};

} /* namespace gerbex */

#endif /* CIRCLE_H_ */
