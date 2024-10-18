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

namespace gerbex {

Draw::Draw() :
		m_segment { }, m_aperture { std::make_shared<Circle>() }, m_transform { } {
	// Empty
}

Draw::Draw(const Segment &segment, std::shared_ptr<Aperture> aperture,
		const ApertureTransformation &transformation) :
		m_segment { segment }, m_aperture { aperture }, m_transform {
				transformation } {
	// Empty
}

Draw::~Draw() {
	// Empty
}

void Draw::Serialize(Serializer &serializer) {
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(
			m_aperture);
	serializer.AddDraw(circle->GetDiameter(), m_segment);
}

std::shared_ptr<Aperture> Draw::GetAperture() const {
	return m_aperture;
}

const Segment& Draw::GetSegment() const {
	return m_segment;
}

const ApertureTransformation& Draw::GetTransform() const {
	return m_transform;
}

} /* namespace gerbex */
