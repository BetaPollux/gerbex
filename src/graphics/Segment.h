/*
 * Segment.h
 *
 *  Created on: Mar. 29, 2024
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

#ifndef SEGMENT_H_
#define SEGMENT_H_

#include "Box.h"
#include "Point.h"
#include "Transform.h"
#include <memory>

namespace gerbex {

/*
 * A graphical object with an origin and end point that can be used to define a contour.
 */
class Segment {
public:
	Segment();
	Segment(const Point &start, const Point &end);
	virtual ~Segment() = default;
	bool operator==(const Segment &rhs) const;
	bool operator!=(const Segment &rhs) const;
	const Point& GetStart() const;
	const Point& GetEnd() const;
	virtual Box GetBox() const;
	virtual std::unique_ptr<Segment> Clone();
	virtual void Translate(const Point &offset);
	virtual void ApplyTransform(const gerbex::Transform &transform);

protected:
	Point m_start, m_end;
};

} /* namespace gerbex */

#endif /* SEGMENT_H_ */
