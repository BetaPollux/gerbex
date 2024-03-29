/*
 * Arc.h
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

#ifndef ARC_H_
#define ARC_H_

#include "GraphicalObject.h"

enum class ArcDirection {
	Clockwise,
	CounterClockwise
};

/*
 * Circular segments drawn with a circle.
 */
class Arc: public GraphicalObject {
public:
	Arc();
	Arc(const Point &origin, const Point &endPoint, const Point &centerOffset, ArcDirection direction,
			std::shared_ptr<Aperture> aperture, const ApertureTransformation &transformation);
	virtual ~Arc();
	const Point& GetCenterOffset() const;
	ArcDirection GetDirection() const;
	const Point& GetEndPoint() const;

private:
	ArcDirection m_direction;
	Point m_endPoint;
	Point m_centerOffset;
};

#endif /* ARC_H_ */
