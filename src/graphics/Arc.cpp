/*
 * Arc.cpp
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

#include "Arc.h"
#include "Circle.h"
#include "Serializer.h"

namespace gerbex {

Arc::Arc() :
		m_segment { }, m_aperture { std::make_shared<Circle>() }, m_transform { } {
	// Empty
}

Arc::Arc(const ArcSegment &segment, std::shared_ptr<Aperture> aperture,
		const ApertureTransformation &transformation) :
		m_segment { segment }, m_aperture { aperture }, m_transform {
				transformation } {
	// Empty
}

void Arc::Serialize(Serializer &serializer, const Point &origin,
		const ApertureTransformation &transform) {
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(
			m_aperture);
	double width = transform.ApplyScaling(circle->GetDiameter());
	ArcSegment segment = m_segment;
	segment.Transform(transform);
	segment.Translate(origin);
	serializer.AddArc(width, segment);
}

std::shared_ptr<Aperture> Arc::GetAperture() const {
	return m_aperture;
}

const ArcSegment& Arc::GetSegment() const {
	return m_segment;
}

const ApertureTransformation& Arc::GetTransform() const {
	return m_transform;
}

Box Arc::GetBox() const {
	//TODO arc GetBox
}

} /* namespace gerbex */
