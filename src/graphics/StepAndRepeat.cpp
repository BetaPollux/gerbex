/*
 * StepAndRepeat.cpp
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

#include "Serializer.h"
#include "StepAndRepeat.h"
#include <stdexcept>

namespace gerbex {

StepAndRepeat::StepAndRepeat() :
		StepAndRepeat(1, 1, 0.0, 0.0) {
	// Empty
}

StepAndRepeat::StepAndRepeat(int nx, int ny, double dx, double dy) :
		m_nx { nx }, m_ny { ny }, m_dx { dx }, m_dy { dy } {
	if (nx < 1 || ny < 1) {
		throw std::invalid_argument("Must repeat at least once.");
	}
	if (dx < 0.0 || dy < 0.0) {
		throw std::invalid_argument("Step size must be zero or positive.");
	}
}

StepAndRepeat::~StepAndRepeat() {
	// Empty
}

void StepAndRepeat::AddObject(std::shared_ptr<GraphicalObject> object) {
	m_objects.push_back(object);
}

std::vector<std::shared_ptr<GraphicalObject>>* StepAndRepeat::GetObjectList() {
	return &m_objects;
}

double StepAndRepeat::GetDx() const {
	return m_dx;
}

double StepAndRepeat::GetDy() const {
	return m_dy;
}

int StepAndRepeat::GetNx() const {
	return m_nx;
}

int StepAndRepeat::GetNy() const {
	return m_ny;
}

void StepAndRepeat::Serialize(Serializer &serializer, const Point &origin,
		const Transform &transform) const {
	for (int ix = 0; ix < m_nx; ix++) {
		for (int iy = 0; iy < m_ny; iy++) {
			for (auto obj : m_objects) {
				Point offset(m_dx * ix, m_dy * iy);
				obj->Serialize(serializer, origin + offset, transform);
			}
		}
	}
}

Box StepAndRepeat::GetBox() const {
	if (m_objects.empty()) {
		throw std::invalid_argument("cannot get box for empty step and repeat");
	}
	Box box = m_objects.front()->GetBox();
	for (int ix = 0; ix < m_nx; ix++) {
		for (int iy = 0; iy < m_ny; iy++) {
			for (auto obj : m_objects) {
				Point offset(m_dx * ix, m_dy * iy);
				Box next = obj->GetBox();
				box = box.Extend(next.Translate(offset));
			}
		}
	}
	return box;
}

} /* namespace gerbex */
