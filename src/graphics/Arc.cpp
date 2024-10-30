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
		m_segment { }, m_aperture { std::make_shared<Circle>() } {
	// Empty
}

Arc::Arc(const ArcSegment &segment, std::shared_ptr<Aperture> aperture) :
		m_segment { segment } {
	std::shared_ptr<Circle> circle = std::dynamic_pointer_cast<Circle>(
			aperture);
	if (!circle) {
		throw std::invalid_argument("arc only supports circle apertures");
	}
	m_aperture = circle;
}

void Arc::Serialize(Serializer &serializer, pSerialItem target, const Point &origin) const {
	double width = m_aperture->GetDiameter();
	ArcSegment segment = m_segment;
	segment.Translate(origin);
	pSerialItem dest = serializer.GetTarget(m_polarity);
	serializer.AddArc(dest, width, segment);
}

std::shared_ptr<Circle> Arc::GetAperture() const {
	return m_aperture;
}

const ArcSegment& Arc::GetSegment() const {
	return m_segment;
}

Box Arc::GetBox() const {
	double radius = 0.5 * m_aperture->GetDiameter();
	Box box = m_segment.GetBox().Pad(radius);
	return box;
}

void Arc::ApplyTransform(const Transform &transform) {
	m_aperture->ApplyTransform(transform);
	m_segment.ApplyTransform(transform);
}

std::unique_ptr<GraphicalObject> Arc::Clone() {
	return std::make_unique<Arc>(*this);
}

void Arc::Translate(const Point &offset) {
	m_segment.Translate(offset);
}

} /* namespace gerbex */
