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
#include "Serializer.h"
#include <algorithm>

namespace gerbex {

Draw::Draw() :
		m_segment { }, m_aperture { std::make_shared<Circle>() } {
	// Empty
}

Draw::Draw(const Segment &segment, std::shared_ptr<Circle> aperture) :
		m_segment { segment }, m_aperture { aperture } {
	// Empty
}

void Draw::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	double width = m_aperture->GetDiameter();
	width = transform.ApplyScaling(width);
	Segment segment = m_segment;
	segment.Transform(transform);
	segment.Translate(origin);
	serializer.AddDraw(width, segment);
}

std::shared_ptr<Circle> Draw::GetAperture() const {
	return m_aperture;
}

const Segment& Draw::GetSegment() const {
	return m_segment;
}

Box Draw::GetBox() const {
	double radius = 0.5 * m_aperture->GetDiameter();
	Box box = m_segment.GetBox().Pad(radius);
	return box;
}

} /* namespace gerbex */
