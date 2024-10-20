/*
 * Box.cpp
 *
 *  Created on: Oct 19, 2024
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
#include "Box.h"
#include <algorithm>

namespace gerbex {

Box::Box() :
		Box(1.0, 1.0, 0.0, 0.0) {
	// Empty
}

Box::Box(double width, double height, double left, double bottom) :
		m_width { width }, m_height { height }, m_left { left }, m_bottom {
				bottom } {
}

double Box::GetHeight() const {
	return m_height;
}

double Box::GetWidth() const {
	return m_width;
}

double Box::GetBottom() const {
	return m_bottom;
}

double Box::GetTop() const {
	return m_bottom + m_height;
}

double Box::GetLeft() const {
	return m_left;
}

double Box::GetRight() const {
	return m_left + m_width;
}

bool Box::operator ==(const Box &rhs) const {
	return m_width == rhs.m_width && m_height == rhs.m_height
			&& m_left == rhs.m_left && m_bottom == rhs.m_bottom;
}

bool Box::operator !=(const Box &rhs) const {
	return m_width != rhs.m_width || m_height != rhs.m_height
			|| m_left != rhs.m_left || m_bottom != rhs.m_bottom;
}

Box Box::Extend(const Box &other) const {
	double left = std::min(GetLeft(), other.GetLeft());
	double bottom = std::min(GetBottom(), other.GetBottom());
	double right = std::max(GetRight(), other.GetRight());
	double top = std::max(GetTop(), other.GetTop());
	return Box(right - left, top - bottom, left, bottom);
}

Box Box::Pad(double pad) const {
	double width = m_width + 2 * pad;
	double height = m_height + 2 * pad;
	double left = m_left - pad;
	double bottom = m_bottom - pad;
	return Box(width, height, left, bottom);
}

Box Box::Translate(const Point &offset) const {
	double left = m_left + offset.GetX();
	double bottom = m_bottom + offset.GetY();
	return Box(m_width, m_height, left, bottom);
}

} /* namespace gerbex */
