/*
 * CoordinateFormat.h
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

#ifndef COORDINATEFORMAT_H_
#define COORDINATEFORMAT_H_

#include "Point.h"

/*
 * Specifies the number of integer and decimal places when interpreting coordinate data.
 * e.g., format of X36Y36 means 3+6 digits, or "X123123456" is 123.123456 mm
 */
class CoordinateFormat {
public:
	CoordinateFormat();
	CoordinateFormat(int integer, int decimal);
	virtual ~CoordinateFormat();
	RealPoint Convert(const Point &point);

private:
	int m_integer;	//1 to 6
	int m_decimal;	//6
	double m_resolution;
};

#endif /* COORDINATEFORMAT_H_ */
