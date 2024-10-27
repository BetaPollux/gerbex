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
#include "Contour.h"
#include "Point.h"
#include "Polarity.h"
#include "Segment.h"
#include <string>
#include <vector>

namespace gerbex {

/*
 *
 */
class Serializer {
public:
	Serializer() = default;
	virtual ~Serializer() = default;
	virtual void AddCircle(double radius, const Point &center) = 0;
	virtual void AddPolygon(const std::vector<Point> &points) = 0;
	virtual void AddDraw(double width, const Segment &segment) = 0;
	virtual void AddArc(double width, const ArcSegment &segment) = 0;
	virtual void AddContour(const Contour &contour) = 0;
	const Polarity& GetPolarity() const {
		return m_polarity;
	}
	void SetPolarity(const Polarity &polarity) {
		m_polarity = polarity;
	}

protected:
	Polarity m_polarity;

};

} /* namespace gerbex */

#endif /* SERIALIZER_H_ */
