/*
 * BlockAperture.cpp
 *
 *  Created on: Mar. 29, 2024
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

#include "BlockAperture.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

BlockAperture::BlockAperture() {
	// Empty

}

BlockAperture::~BlockAperture() {
	// Empty
}

void BlockAperture::AddObject(std::shared_ptr<GraphicalObject> object) {
	m_objects.push_back(object);
}

std::vector<std::shared_ptr<GraphicalObject>>* BlockAperture::GetObjectList() {
	return &m_objects;
}

void BlockAperture::Serialize(Serializer &serializer, const Point &origin,
		const ApertureTransformation &transform) {
	for (auto obj : m_objects) {
		obj->Serialize(serializer, origin, transform);
	}
}

Box BlockAperture::GetBox() const {
	//TODO block getbox
	return Box();
}

} /* namespace gerbex */
