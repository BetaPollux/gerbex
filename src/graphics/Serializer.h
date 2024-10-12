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

#include <string>
#include <vector>

namespace gerbex {

//TODO use RealPoint here

/*
 *
 */
class Serializer {
public:
	Serializer() : m_xOffset { 0.0 }, m_yOffset { 0.0 } {}
	virtual ~Serializer() {}
	virtual void SetOffset(double x, double y) {
		//TODO change all graphics to use RealPoint, apply Format spec
		m_xOffset = x * 1e-6;
		m_yOffset = y * 1e-6;
	}
	virtual void AddCircle(double radius, double centerX, double centerY) = 0;
	virtual void AddRectangle(double width, double height, double left, double top) = 0;
	virtual void AddPolygon(const std::vector<std::pair<double, double>> &points) = 0;
	virtual void AddDraw(double width, double x1, double y1, double x2, double y2) = 0;
	//TODO add arc
	//TODO add path builder
	//TODO add polarity & transform

protected:
	double m_xOffset, m_yOffset;
};


class Serializable {
public:
	Serializable() {}
	virtual ~Serializable() {}
	virtual void Serialize(Serializer &serializer) = 0;
};

} /* namespace gerbex */

#endif /* SERIALIZER_H_ */
