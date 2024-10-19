/*
 * Polygon.h
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

#ifndef POLYGON_H_
#define POLYGON_H_

#include "Aperture.h"

namespace gerbex {

/*
 * Polygon aperture with optional hole.
 * It is a regular polygon (all angles equal) with 3 to 12 vertices.
 */
class Polygon: public Aperture {
public:
	Polygon();
	Polygon(double outerDiameter, int numVertices, double rotation = 0.0, double holeDiameter = 0.0);
	virtual ~Polygon();
	double GetHoleDiameter() const;
	int GetNumVertices() const;
	double GetOuterDiameter() const;
	double GetRotation() const;
	void Serialize(Serializer &serializer, const Point &origin,
			const ApertureTransformation &transform) override;

private:
	double m_outerDiameter;
	int m_numVertices;
	double m_rotation;
	double m_holeDiameter;
};

} /* namespace gerbex */

#endif /* POLYGON_H_ */
