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

namespace gerbex {

class CoordinateData {
public:
	CoordinateData(std::optional<FixedPointType> x,
			std::optional<FixedPointType> y, std::optional<FixedPoint> ij =
					std::nullopt);
	CoordinateData();
	virtual ~CoordinateData();
	static CoordinateData FromString(const std::string &str);
	FixedPoint GetXY(const std::optional<FixedPoint> &defaultPt = std::nullopt) const;
	bool HasXY() const;
	bool HasIJ() const;
	const std::optional<FixedPoint>& GetIJ() const;
	const std::optional<FixedPointType>& GetX() const;
	const std::optional<FixedPointType>& GetY() const;

private:
	std::optional<FixedPointType> m_x;
	std::optional<FixedPointType> m_y;
	std::optional<FixedPoint> m_ij;

};

} /* namespace gerbex */

#endif /* COORDINATEDATA_H_ */
