/*
 * Flash.cpp
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
#include "Flash.h"
#include "Serializer.h"

namespace gerbex {

Flash::Flash() :
		m_origin { }, m_aperture { std::make_shared<Circle>() } {
	// Empty

}

Flash::Flash(const Point &origin, std::shared_ptr<Aperture> aperture) :
		m_origin { origin }, m_aperture { aperture } {
	// Empty
}

void Flash::Serialize(Serializer &serializer, const Point &origin) const {
	serializer.SetPolarity(m_polarity);
	m_aperture->Serialize(serializer, m_origin + origin);
}

std::shared_ptr<Aperture> Flash::GetAperture() const {
	return m_aperture;
}

const Point& Flash::GetOrigin() const {
	return m_origin;
}

Box Flash::GetBox() const {
	return m_aperture->GetBox().Translate(m_origin);
}

void Flash::ApplyTransform(const Transform &transform) {
	m_aperture->ApplyTransform(transform);
	m_origin.ApplyTransform(transform);
}

std::unique_ptr<GraphicalObject> Flash::Clone() {
	return std::make_unique<Flash>(*this);
}

void Flash::Translate(const Point &offset) {
	m_origin += offset;
}

} /* namespace gerbex */
