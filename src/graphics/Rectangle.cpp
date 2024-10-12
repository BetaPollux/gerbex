/*
 * Rectangle.cpp
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

#include "Rectangle.h"
#include "Serializer.h"
#include <stdexcept>

namespace gerbex {

Rectangle::Rectangle()
	: Rectangle{ 1.0, 1.0 }
{
	// Empty
}

Rectangle::Rectangle(double xSize, double ySize, double holeDiameter)
	: m_xSize{ xSize },
	  m_ySize{ ySize },
	  m_holeDiameter{ holeDiameter }
{
	if (xSize <= 0.0 || ySize <= 0.0) {
		throw std::invalid_argument("Size must be > 0");
	}

	if (holeDiameter < 0.0) {
		throw std::invalid_argument("Hole diameter must be >= 0");
	}
}

Rectangle::~Rectangle() {
	// Empty
}

double Rectangle::GetHoleDiameter() const {
	return m_holeDiameter;
}

double Rectangle::GetXSize() const {
	return m_xSize;
}

double Rectangle::GetYSize() const {
	return m_ySize;
}

void Rectangle::Serialize(Serializer &serializer) {
	//TODO fix top coord
	serializer.AddRectangle(m_xSize, m_ySize, Point(-0.5 * m_xSize, -0.5 * m_ySize));
}

} /* namespace gerbex */

