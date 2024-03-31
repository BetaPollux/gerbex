/*
 * Rectangle.h
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

#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Aperture.h"

/*
 * Rectangle aperture with optional hole.
 */
class Rectangle: public Aperture {
public:
	Rectangle();
	Rectangle(double xSize, double ySize, double holeDiameter = 0.0);
	virtual ~Rectangle();
	double GetHoleDiameter() const;
	double GetXSize() const;
	double GetYSize() const;

private:
	double m_xSize;
	double m_ySize;
	double m_holeDiameter;
};

#endif /* RECTANGLE_H_ */
