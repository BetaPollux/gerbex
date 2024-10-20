/*
 * Obround.h
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

#ifndef OBROUND_H_
#define OBROUND_H_

#include "Aperture.h"

namespace gerbex {

/*
 * Obround aperture with optional hole.
 * An obround is a rectangle where the smallest side is rounded to a half-circle.
 */
class Obround: public Aperture {
public:
	Obround();
	Obround(double xSize, double ySize, double holeDiameter = 0.0);
	virtual ~Obround() = default;
	double GetHoleDiameter() const;
	double GetXSize() const;
	double GetYSize() const;
	void Serialize(Serializer &serializer, const Point &origin,
			const ApertureTransformation &transform) const override;
	Box GetBox() const override;

private:
	double m_xSize;
	double m_ySize;
	double m_holeDiameter;
};

} /* namespace gerbex */

#endif /* OBROUND_H_ */
