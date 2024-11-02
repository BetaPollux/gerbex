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

void BlockAperture::AddObject(std::shared_ptr<GraphicalObject> object) {
	m_objects.push_back(object);
}

std::vector<std::shared_ptr<GraphicalObject>>* BlockAperture::GetObjectList() {
	return &m_objects;
}

void BlockAperture::Serialize(Serializer &serializer, pSerialItem target, const Point &origin) const {
	(void)target;
	for (auto obj : m_objects) {
		obj->Serialize(serializer, origin);
	}
}

Box BlockAperture::GetBox() const {
	if (m_objects.empty()) {
		throw std::invalid_argument("cannot get box for empty block aperture");
	}
	Box box = m_objects.front()->GetBox();
	for (std::shared_ptr<GraphicalObject> obj : m_objects) {
		box = box.Extend(obj->GetBox());
	}
	return box;
}

size_t BlockAperture::GetObjectCount() const {
	return m_objects.size();
}

std::unique_ptr<Aperture> BlockAperture::Clone() const {
	std::unique_ptr<BlockAperture> block = std::make_unique<BlockAperture>();
	block->m_objects.reserve(m_objects.size());
	for (std::shared_ptr<GraphicalObject> obj : m_objects) {
		block->m_objects.push_back(obj->Clone());
	}
	return block;
}

void BlockAperture::ApplyTransform(const Transform &transform) {
	for (std::shared_ptr<GraphicalObject> obj : m_objects) {
		obj->ApplyTransform(transform);
	}
}

} /* namespace gerbex */
