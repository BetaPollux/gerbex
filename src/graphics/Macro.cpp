/*
 * Macro.cpp
 *
 *  Created on: Mar. 30, 2024
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

#include "Macro.h"
#include "Serializer.h"

namespace gerbex {

Macro::Macro() {
	// Empty

}

void Macro::AddPrimitive(std::shared_ptr<MacroPrimitive> primitive) {
	m_primitives.push_back(primitive);
}

const std::vector<std::shared_ptr<MacroPrimitive> >& Macro::GetPrimitives() const {
	return m_primitives;
}

void Macro::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	for (auto prim : m_primitives) {
		prim->Serialize(serializer, origin, m_transform.Stack(transform));
	}
}

Box Macro::GetBox() const {
	if (m_primitives.empty()) {
		throw std::invalid_argument("cannot get box for empty macro");
	}
	Box box = m_primitives.front()->GetBox();
	for (std::shared_ptr<MacroPrimitive> obj : m_primitives) {
		box = box.Extend(obj->GetBox());
	}
	return box;
}

std::unique_ptr<Aperture> Macro::Clone() const {
	return std::make_unique<Macro>(*this);
}

} /* namespace gerbex */
