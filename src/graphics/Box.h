/*
 * Box.h
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
#ifndef BOX_H_
#define BOX_H_

#include "Point.h"
#include <ostream>
#include <vector>

namespace gerbex {

class Box {
public:
	Box();
	Box(double width, double height, double left, double bottom);
	Box(const std::vector<Point> &points);
	Box(double diameter, const Point& center);
	virtual ~Box() = default;
	bool operator==	(const Box& rhs) const;
	bool operator!=	(const Box& rhs) const;
	double GetHeight() const;
	double GetWidth() const;
	double GetBottom() const;
	double GetTop() const;
	double GetLeft() const;
	double GetRight() const;
	double GetAspectRatio() const;
	Box Extend(const Box &other) const;
	Box Pad(double pad) const;
	Box Translate(const Point &offset) const;
	friend std::ostream& operator<<(std::ostream& os, const Box& box);

private:
	double m_width, m_height;
	double m_left, m_bottom;
};

} /* namespace gerbex */

#endif /* BOX_H_ */
