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

double Box::GetLeft() const {
	return m_left;
}

Box Box::Extend(const Box &other) {
	double width = std::max(m_width, other.m_width);
	double height = std::max(m_height, other.m_height);
	double left = std::min(m_left, other.m_left);
	double bottom = std::min(m_bottom, other.m_bottom);
	return Box(width, height, left, bottom);
}

} /* namespace gerbex */
