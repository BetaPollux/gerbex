/*
 * Point.h
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

#ifndef POINT_H_
#define POINT_H_

#include <cstdint>

namespace gerbex {

/*
 *	Point in the plane.
 *
 *	Point:
 *  Coordinates are in fixed format decimals, to be converted to real units based on
 *  the Coordinate Format and Unit options.
 *
 *	RealPoint:
 *	Coordinates in real units, e.g. inches or millimeters.
 *
 *  Orientation:
 *		Origin is bottom-left.
 *		X points to the right.
 *		Y points upwards.
 */
template<typename T>
class GenericPoint {
public:
	GenericPoint() {
	}
	GenericPoint(T x, T y) :
			m_x { x }, m_y { y } {
	}
	virtual ~GenericPoint() {
	}
	bool operator==(const GenericPoint<T> &rhs) const {
		return (m_x == rhs.m_x) && (m_y == rhs.m_y);
	}
	bool operator!=(const GenericPoint<T> &rhs) const {
		return (m_x != rhs.m_x) || (m_y != rhs.m_y);
	}
	GenericPoint operator+(const GenericPoint<T> &rhs) const {
		return GenericPoint<T>(m_x + rhs.m_x, m_y + rhs.m_y);
	}
	GenericPoint operator-(const GenericPoint<T> &rhs) const {
		return GenericPoint<T>(m_x - rhs.m_x, m_y - rhs.m_y);
	}
	GenericPoint operator*(T factor) const {
		return GenericPoint<T>(m_x * factor, m_y * factor);
	}
	T GetX() const {
		return m_x;
	}
	T GetY() const {
		return m_y;
	}

private:
	T m_x, m_y;
};

using PointCoordType = int32_t;
using Point = GenericPoint<PointCoordType>;
using RealPoint = GenericPoint<double>;

} /* namespace gerbex */

#endif /* POINT_H_ */
