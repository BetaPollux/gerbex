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

#include "GraphicalObject.h"

/*
 * A graphical object with an origin and end point that can be used to define a contour.
 */
class Segment: public GraphicalObject {
public:
	Segment();
	Segment(const Point &origin, const Point &endPoint);
	Segment(const Point &origin, const Point &endPoint,
			std::shared_ptr<Aperture> aperture, const ApertureTransformation &transformation);
	virtual ~Segment();
	const Point& GetEndPoint() const;

protected:
	Point m_endPoint;
};

#endif /* SEGMENT_H_ */
