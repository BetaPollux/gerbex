/*
 * Coordinate.cpp
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

#include <regex>
#include "Coordinate.h"

Coordinate::Coordinate() :
		Coordinate(0, 0) {
	// Empty
}

Coordinate::Coordinate(std::optional<PointCoordType> x,
		std::optional<PointCoordType> y, std::optional<Point> offset) :
		m_x { x }, m_y { y }, m_offset { offset } {
	// Empty
}

Coordinate::~Coordinate() {
}

Coordinate Coordinate::FromString(const std::string &str) {
	std::smatch match;
	// Find [X][Y][I J]
	std::regex pattern("(X(\\d+))?(Y(\\d+))?(I(\\d+)J(\\d+))?");
	std::regex_search(str, match, pattern);

	std::optional<PointCoordType> x, y;
	std::optional<Point> offset;
	// X-value
	if (match[2].matched) {
		x = std::stoi(match[2].str());
	}

	// Y-value
	if (match[4].matched) {
		y = std::stoi(match[4].str());
	}

	// IJ-value
	if (match[5].matched) {
		PointCoordType i = std::stoi(match[6].str());
		PointCoordType j = std::stoi(match[7].str());
		offset = Point(i, j);
	}

	return Coordinate(x, y, offset);
}

bool Coordinate::HasXY() const {
	return m_x.has_value() && m_y.has_value();
}

bool Coordinate::HasOffset() const {
	return m_offset.has_value();
}

const std::optional<Point>& Coordinate::GetOffset() const {
	return m_offset;
}

const std::optional<PointCoordType>& Coordinate::GetX() const {
	return m_x;
}

const std::optional<PointCoordType>& Coordinate::GetY() const {
	return m_y;
}
