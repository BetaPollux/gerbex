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
#include <ostream>

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

using FixedPointType = int32_t;

class FixedPoint {
public:
	FixedPoint() :
			FixedPoint(0, 0) {
	}
	FixedPoint(FixedPointType x, FixedPointType y) :
			m_x { x }, m_y { y } {
	}
	virtual ~FixedPoint() = default;
	FixedPointType GetX() const {
		return m_x;
	}
	void SetX(FixedPointType x) {
		m_x = x;
	}
	FixedPointType GetY() const {
		return m_y;
	}
	void SetY(FixedPointType y) {
		m_y = y;
	}

private:
	FixedPointType m_x, m_y;
};

class Point {
public:
	Point() :
			Point(0.0, 0.0) {
	}
	Point(double x, double y) :
			m_x { x }, m_y { y } {
	}
	virtual ~Point() = default;
	bool operator==(const Point &rhs) const {
		return fabs(m_x - rhs.m_x) <= kEqualityThreshold
				&& fabs(m_y - rhs.m_y) <= kEqualityThreshold;
	}
	bool operator!=(const Point &rhs) const {
		return fabs(m_x - rhs.m_x) > kEqualityThreshold
				|| fabs(m_y - rhs.m_y) > kEqualityThreshold;
	}
	Point operator+(const Point &rhs) const {
		return Point(m_x + rhs.m_x, m_y + rhs.m_y);
	}
	Point& operator+=(const Point &rhs) {
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		return *this;
	}
	Point operator-(const Point &rhs) const {
		return Point(m_x - rhs.m_x, m_y - rhs.m_y);
	}
	Point& operator-=(const Point &rhs) {
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		return *this;
	}
	Point operator*(double factor) const {
		return Point(m_x * factor, m_y * factor);
	}
	Point operator/(double factor) const {
		return Point(m_x / factor, m_y / factor);
	}
	Point& operator*=(double factor) {
		m_x *= factor;
		m_y *= factor;
		return *this;
	}
	double GetX() const {
		return m_x;
	}
	double GetY() const {
		return m_y;
	}
	void Rotate(double degrees) {
		if (degrees != 0.0) {
			double rad = M_PI * degrees / 180.0;
			double newX = cos(rad) * m_x - sin(rad) * m_y;
			double newY = sin(rad) * m_x + cos(rad) * m_y;
			m_x = newX;
			m_y = newY;
		}
	}
	double Distance(const Point &rhs) const {
		double dx = m_x - rhs.m_x;
		double dy = m_y - rhs.m_y;
		return sqrt(dx * dx + dy * dy);
	}
	void SetX(double x) {
		m_x = x;
	}
	void SetY(double y) {
		m_y = y;
	}
	friend std::ostream& operator<<(std::ostream& os, const Point& point);

private:
	static constexpr double kEqualityThreshold = 1e-9;
	double m_x, m_y;
};

} /* namespace gerbex */

#endif /* POINT_H_ */
