/*
 * CoordinateData.h
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

#ifndef COORDINATEDATA_H_
#define COORDINATEDATA_H_

#include <optional>
#include <string>
#include "Point.h"

class CoordinateData {
public:
	CoordinateData(std::optional<PointCoordType> x,
			std::optional<PointCoordType> y, std::optional<Point> offset =
					std::nullopt);
	CoordinateData();
	virtual ~CoordinateData();
	static CoordinateData FromString(const std::string &str);
	static std::optional<Point> FromDefaults(const CoordinateData &newData,
			const std::optional<Point> &defaultPt = std::nullopt);
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

#endif /* COORDINATEDATA_H_ */
