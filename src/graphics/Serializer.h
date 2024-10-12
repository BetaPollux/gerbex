/*
 * Serializer.h
 *
 *  Created on: Oct. 11, 2024
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

#ifndef SERIALIZER_H_
#define SERIALIZER_H_

#include "Arc.h"
#include "Point.h"
#include <string>
#include <vector>

namespace gerbex {

/*
 *
 */
class Serializer {
public:
	Serializer() :
			m_xOffset { 0.0 }, m_yOffset { 0.0 } {
	}
	virtual ~Serializer() {
	}
	virtual void SetOffset(double x, double y) {
		m_xOffset = x;
		m_yOffset = y;
	}
	virtual void AddCircle(double radius, const Point &center) = 0;
	virtual void AddRectangle(double width, double height,
			const Point &topLeft) = 0;
	virtual void AddObround(double width, double height,
			const Point &center) = 0;
	virtual void AddPolygon(const std::vector<Point> &points) = 0;
	virtual void AddDraw(double width, const Point &start,
			const Point &end) = 0;
	virtual void AddArc(double width, const Point &start, const Point &end,
			const Point &center, ArcDirection direction) = 0;
	//TODO add path builder
	//TODO add polarity & transform

protected:
	double m_xOffset, m_yOffset;
};

} /* namespace gerbex */

#endif /* SERIALIZER_H_ */
