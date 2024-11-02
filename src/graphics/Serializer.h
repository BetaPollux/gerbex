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

#include "GraphicalObject.h"
#include <memory>
#include <string>
#include <vector>

namespace gerbex {

class SerialItem {
public:
	virtual ~SerialItem() = default;
};

using pSerialItem = std::shared_ptr<SerialItem>;

class ArcSegment;
class Box;
class Contour;
class Point;
class Segment;

/*
 *
 */
class Serializer {
public:
	Serializer() = default;
	virtual ~Serializer() = default;
	virtual pSerialItem NewGroup(pSerialItem parent) = 0;
	virtual pSerialItem NewMask(const Box &box) = 0;
	virtual void SetMask(pSerialItem target, pSerialItem mask) = 0;
	virtual pSerialItem AddArc(pSerialItem target, double width,
			const ArcSegment &segment) = 0;
	virtual pSerialItem AddCircle(pSerialItem target, double radius,
			const Point &center) = 0;
	virtual pSerialItem AddContour(pSerialItem target,
			const Contour &contour) = 0;
	virtual pSerialItem AddDraw(pSerialItem target, double width,
			const Segment &segment) = 0;
	virtual pSerialItem AddPolygon(pSerialItem target,
			const std::vector<Point> &points) = 0;
	virtual pSerialItem GetTarget(Polarity polarity) = 0;
};

} /* namespace gerbex */

#endif /* SERIALIZER_H_ */
