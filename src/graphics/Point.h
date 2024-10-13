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

#include <cmath>
#include <cstdint>

namespace gerbex {

/*
 *	Point in the plane.
 *
 *	FixedPoint:
 *  Coordinates are in fixed format decimals, to be converted to real units based on
 *  the Coordinate Format and Unit options.
 *
 *	Point:
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
	GenericPoint operator*(double factor) const {
		return GenericPoint<T>(m_x * factor, m_y * factor);
	}
	T GetX() const {
		return m_x;
	}
	T GetY() const {
		return m_y;
	}
	void Rotate(double degrees) {
		//TODO add a rotate around a reference point
		if (degrees != 0.0) {
			double rad = M_PI * degrees / 180.0;
			T newX = cos(rad) * m_x - sin(rad) * m_y;
			T newY = sin(rad) * m_x + cos(rad) * m_y;
			m_x = newX;
			m_y = newY;
		}
	}

private:
	T m_x, m_y;
};

// TODO double needs comparison with tolerance

using FixedPointType = int32_t;
using FixedPoint = GenericPoint<FixedPointType>;
using PointType = double;
using Point = GenericPoint<PointType>;

} /* namespace gerbex */

#endif /* POINT_H_ */
