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

Draw::Draw(const Segment &segment, std::shared_ptr<Aperture> aperture) :
		m_segment { segment } {
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(aperture);
	if (!circle) {
		throw std::invalid_argument("draw only supports circle apertures");
	}
	m_aperture = circle;
}

void Draw::Serialize(Serializer &serializer, const Point &origin) const {
	double width = m_aperture->GetDiameter();
	Segment segment = m_segment;
	segment.Translate(origin);
	//TODO draw should have its own polarity?
	serializer.AddDraw(width, segment, Polarity::Dark);
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

void Draw::ApplyTransform(const Transform &transform) {
	m_aperture->ApplyTransform(transform);
	m_segment.Transform(transform);
}

std::unique_ptr<GraphicalObject> Draw::Clone() {
	return std::make_unique<Draw>(*this);
}

void Draw::Translate(const Point &offset) {
	m_segment.Translate(offset);
}

} /* namespace gerbex */
