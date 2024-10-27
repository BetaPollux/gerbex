/*
 * ArcSegment.h
 *
 *  Created on: Oct. 15, 2024
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
#ifndef ARCSEGMENT_H_
#define ARCSEGMENT_H_

#include "Segment.h"
#include "Transform.h"

namespace gerbex {

enum class ArcDirection {
	Clockwise, CounterClockwise
};

class ArcSegment: public Segment {
public:
	ArcSegment();
	ArcSegment(const Point &start, const Point &end, const Point &centerOffset,
			ArcDirection direction);
	virtual ~ArcSegment() = default;
	bool operator==(const ArcSegment &rhs) const;
	bool operator!=(const ArcSegment &rhs) const;
	const Point& GetCenterOffset() const;
	Point GetCenter() const;
	ArcDirection GetDirection() const;
	bool IsCircle() const;
	double GetRadius() const;
	void ApplyTransform(const gerbex::Transform &transform) override;
	Box GetBox() const override;
	std::unique_ptr<Segment> Clone() override;

protected:
	Point m_centerOffset;
	ArcDirection m_direction;
};

} /* namespace gerbex */

#endif /* ARCSEGMENT_H_ */
