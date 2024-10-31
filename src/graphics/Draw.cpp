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
		m_segment { }, m_drawWidth { 1.0 } {
	// Empty
}

Draw::Draw(const Segment &segment, std::shared_ptr<Aperture> aperture) :
		m_segment { segment } {
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(aperture);
	if (!circle) {
		throw std::invalid_argument("draw only supports circle apertures");
	}
	m_drawWidth = circle->GetDiameter();
}

void Draw::Serialize(Serializer &serializer, pSerialItem target, const Point &origin) const {
	Segment segment = m_segment;
	segment.Translate(origin);
	pSerialItem dest = serializer.GetTarget(m_polarity);
	serializer.AddDraw(dest, m_drawWidth, segment);
}

double Draw::GetDrawWidth() const {
	return m_drawWidth;
}

const Segment& Draw::GetSegment() const {
	return m_segment;
}

Box Draw::GetBox() const {
	double radius = 0.5 * m_drawWidth;
	Box box = m_segment.GetBox().Pad(radius);
	return box;
}

void Draw::ApplyTransform(const Transform &transform) {
	m_drawWidth *= transform.GetScaling();
	m_segment.ApplyTransform(transform);
}

std::unique_ptr<GraphicalObject> Draw::Clone() {
	return std::make_unique<Draw>(*this);
}

void Draw::Translate(const Point &offset) {
	m_segment.Translate(offset);
}

} /* namespace gerbex */
