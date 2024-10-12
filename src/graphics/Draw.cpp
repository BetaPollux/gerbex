/*
 * Draw.cpp
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

#include "Circle.h"
#include "Draw.h"

namespace gerbex {

Draw::Draw() {
	// Empty
}

Draw::Draw(const Point &origin, const Point &endPoint) :
		Segment(origin, endPoint) {
	// Empty
}

Draw::Draw(const Point &origin, const Point &endPoint,
		std::shared_ptr<Aperture> aperture,
		const ApertureTransformation &transformation) :
		Segment(origin, endPoint, aperture, transformation) {
	// Empty
}

Draw::~Draw() {
	// Empty
}

void Draw::Serialize(Serializer &serializer) {
	// TODO should not need to reset offset
	serializer.SetOffset(0.0, 0.0);
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(
			m_aperture);
	// TODO needs to use RealPoint
	serializer.AddDraw(circle->GetDiameter(), m_origin.GetX() * 1e-6,
			m_origin.GetY() * 1e-6, m_endPoint.GetX() * 1e-6,
			m_endPoint.GetY() * 1e-6);
}

} /* namespace gerbex */
