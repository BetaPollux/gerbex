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

#include "ApertureTransformation.h"
#include "Arc.h"
#include "MacroPrimitive.h"
#include "Point.h"
#include "Segment.h"
#include <string>
#include <vector>

namespace gerbex {

/*
 *
 */
class Serializer {
public:
	Serializer() :
			m_offset { }, m_offsetStack { }, m_transform { }, m_transformStack { } {
	}
	virtual ~Serializer() = default;
	virtual void PushOffset(const Point &delta) {
		m_offsetStack.push_back(delta);
		updateOffset();
	}
	virtual void PopOffset() {
		m_offsetStack.pop_back();
		updateOffset();
	}
	virtual void PushTransform(const ApertureTransformation &transform) {
		m_transformStack.push_back(transform);
		updateTransform();
	}
	virtual void PopTransform() {
		m_transformStack.pop_back();
		updateTransform();
	}
	virtual void AddCircle(double radius, const Point &center) = 0;
	virtual void AddRectangle(double width, double height,
			const Point &topLeft) = 0;
	virtual void AddObround(double width, double height,
			const Point &center) = 0;
	virtual void AddPolygon(const std::vector<Point> &points) = 0;
	virtual void AddDraw(double width, const Segment &segment) = 0;
	virtual void AddArc(double width, const ArcSegment &segment) = 0;
	virtual void AddContour(
			const std::vector<std::shared_ptr<Segment>> &segments) = 0;

protected:
	virtual void updateOffset() {
		m_offset = Point();
		for (const Point &pt : m_offsetStack) {
			m_offset += pt;
		}
	}
	virtual void updateTransform() {
		m_transform = ApertureTransformation();
		for (const ApertureTransformation &t : m_transformStack) {
			m_transform.Stack(t);
		}
	}
	Point m_offset;
	std::vector<Point> m_offsetStack;
	ApertureTransformation m_transform;
	std::vector<ApertureTransformation> m_transformStack;
};

} /* namespace gerbex */

#endif /* SERIALIZER_H_ */
