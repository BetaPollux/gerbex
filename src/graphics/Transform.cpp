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

#include <Transform.h>
#include <stdexcept>

namespace gerbex {

Transform::Transform() :
		m_isDark { true }, m_mirrorX { false }, m_mirrorY { false }, m_rotation {
				0.0 }, m_scaling { 1.0 } {
	// Empty
}

Transform::Transform(Polarity polarity, Mirroring mirroring, double rotation,
		double scaling) :
		m_rotation { rotation }, m_scaling { scaling } {
	SetPolarity(polarity);
	SetMirroring(mirroring);
}

bool Transform::operator ==(const Transform &rhs) const {
	return (m_isDark == rhs.m_isDark) && (m_mirrorX == rhs.m_mirrorX)
			&& (m_mirrorY == rhs.m_mirrorY) && (m_rotation == rhs.m_rotation)
			&& (m_scaling == rhs.m_scaling);
}

bool Transform::operator !=(const Transform &rhs) const {
	return (m_isDark != rhs.m_isDark) || (m_mirrorX != rhs.m_mirrorX)
			|| (m_mirrorY != rhs.m_mirrorY) || (m_rotation != rhs.m_rotation)
			|| (m_scaling != rhs.m_scaling);
}

Mirroring Transform::GetMirroring() const {
	if (m_mirrorX && m_mirrorY) {
		return Mirroring::XY;
	} else if (m_mirrorX) {
		return Mirroring::X;
	} else if (m_mirrorY) {
		return Mirroring::Y;
	} else {
		return Mirroring::None;
	}
}

void Transform::SetMirroring(Mirroring mirroring) {
	m_mirrorX = mirroring == Mirroring::X || mirroring == Mirroring::XY;
	m_mirrorY = mirroring == Mirroring::Y || mirroring == Mirroring::XY;
}

Polarity Transform::GetPolarity() const {
	return m_isDark ? Polarity::Dark : Polarity::Clear;
}

void Transform::SetPolarity(Polarity polarity) {
	m_isDark = polarity == Polarity::Dark;
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

Polarity Transform::PolarityFromCommand(const std::string &str) {
	if (str == "C") {
		return Polarity::Clear;
	} else if (str == "D") {
		return Polarity::Dark;
	} else {
		throw std::invalid_argument("invalid polarity");
	}
}

double Transform::ApplyScaling(double value) const {
	return value * m_scaling;
}

Transform Transform::Stack(const Transform &transform) const {
	Transform result = *this;
	result.m_scaling = this->m_scaling * transform.m_scaling;
	result.m_rotation = this->m_rotation + transform.m_rotation;
	if (!transform.m_isDark) {
		result.m_isDark = !this->m_isDark;
	}
	if (transform.m_mirrorX) {
		result.m_mirrorX = !this->m_mirrorX;
	}
	if (transform.m_mirrorY) {
		result.m_mirrorY = !this->m_mirrorY;
	}
	return result;
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

Point Transform::Apply(const Point &point) const {
	Point result = point;
	if (m_mirrorX) {
		result.SetX(-result.GetX());
	}
	if (m_mirrorY) {
		result.SetY(-result.GetY());
	}
	result *= m_scaling;
	result.Rotate(m_rotation);
	return result;
}

std::vector<Point> Transform::ApplyThenTranslate(
		const std::vector<Point> &points, const Point &offset) const {
	std::vector<Point> result;
	result.reserve(points.size());
	for (const Point &p : points) {
		Point r = Apply(p);
		r += offset;
		result.push_back(r);
	}
	return result;
}

} /* namespace gerbex */

