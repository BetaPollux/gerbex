/*
 * Transform.cpp
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

#include "Transform.h"
#include <stdexcept>

namespace gerbex {

Transform::Transform() :
		Transform(Mirroring::None, 0.0, 1.0) {
	// Empty
}

Transform::Transform(Mirroring mirroring, double rotation, double scaling) :
		m_mirroring { mirroring }, m_rotation { rotation }, m_scaling { scaling } {
	// Empty
}

bool Transform::operator ==(const Transform &rhs) const {
	return (m_mirroring == rhs.m_mirroring) && (m_rotation == rhs.m_rotation)
			&& (m_scaling == rhs.m_scaling);
}

bool Transform::operator !=(const Transform &rhs) const {
	return (m_mirroring != rhs.m_mirroring) || (m_rotation != rhs.m_rotation)
			|| (m_scaling != rhs.m_scaling);
}

Mirroring Transform::GetMirroring() const {
	return m_mirroring;
}

void Transform::SetMirroring(Mirroring mirroring) {
	m_mirroring = mirroring;
}

double Transform::GetRotation() const {
	return m_rotation;
}

void Transform::SetRotation(double degrees) {
	m_rotation = degrees;
}

double Transform::GetScaling() const {
	return m_scaling;
}

void Transform::SetScaling(double factor) {
	if (factor <= 0.0) {
		throw std::invalid_argument("scaling factor must be > 0");
	}
	m_scaling = factor;
}

Mirroring Transform::MirroringFromCommand(const std::string &str) {
	if (str == "N") {
		return Mirroring::None;
	} else if (str == "X") {
		return Mirroring::X;
	} else if (str == "Y") {
		return Mirroring::Y;
	} else if (str == "XY") {
		return Mirroring::XY;
	} else {
		throw std::invalid_argument("invalid mirroring");
	}
}

} /* namespace gerbex */

