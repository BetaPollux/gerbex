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

Macro::~Macro() {
	// Empty
}

void Macro::AddPrimitive(std::shared_ptr<MacroPrimitive> primitive) {
	m_primitives.push_back(primitive);
}

const std::vector<std::shared_ptr<MacroPrimitive> >& Macro::GetPrimitives() const {
	return m_primitives;
}

void Macro::Serialize(Serializer &serializer, const Point &origin,
		const ApertureTransformation &transform) {
	for (auto prim : m_primitives) {
		prim->Serialize(serializer, origin, transform);
	}
}

} /* namespace gerbex */
