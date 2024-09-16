/*
 * Coordinate.h
 *
 *  Created on: Sep. 15, 2024
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

#ifndef COORDINATE_H_
#define COORDINATE_H_

#include <optional>
#include <string>
#include "Point.h"

class Coordinate {
public:
	Coordinate(std::optional<PointCoordType> x, std::optional<PointCoordType> y,
			std::optional<Point> offset = std::nullopt);
	Coordinate();
	virtual ~Coordinate();
	static Coordinate FromString(const std::string &str);
	bool HasXY() const;
	bool HasOffset() const;
	const std::optional<Point>& GetOffset() const;
	const std::optional<PointCoordType>& GetX() const;
	const std::optional<PointCoordType>& GetY() const;

private:
	std::optional<PointCoordType> m_x;
	std::optional<PointCoordType> m_y;
	std::optional<Point> m_offset;

};

#endif /* COORDINATE_H_ */
