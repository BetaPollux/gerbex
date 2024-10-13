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
#include <vector>
#include <string>
#include <vector>

namespace gerbex {

/*
 *
 */
class Serializer {
public:
	Serializer() :
			m_offset { }, m_offsetStack { }, m_rotation { }, m_rotationStack {}  {
	}
	virtual ~Serializer() {
	}
	virtual void PushOffset(const Point &delta) {
		m_offsetStack.push_back(delta);
		updateOffset();
	}
	virtual void PopOffset() {
		m_offsetStack.pop_back();
		updateOffset();
	}
	virtual void PushRotation(double degrees) {
		m_rotationStack.push_back(degrees);
		updateRotation();
	}
	virtual void PopRotation() {
		m_rotationStack.pop_back();
		updateRotation();
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
	virtual void updateOffset() {
		m_offset = Point();
		for (const Point &pt : m_offsetStack) {
			m_offset = m_offset + pt;
		}
	}
	virtual void updateRotation() {
		m_rotation = 0;
		for (double r : m_rotationStack) {
			m_rotation += r;
		}
	}
	Point m_offset;
	std::vector<Point> m_offsetStack;
	double m_rotation;
	std::vector<double> m_rotationStack;
};

} /* namespace gerbex */

#endif /* SERIALIZER_H_ */
