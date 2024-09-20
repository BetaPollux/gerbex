/*
 * CoordinateData.cpp
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

#include "CoordinateData.h"
#include "DataTypeParser.h"

#include <ostream>
#include <regex>

CoordinateData::CoordinateData() :
		CoordinateData(0, 0) {
	// Empty
}

CoordinateData::CoordinateData(std::optional<PointCoordType> x,
		std::optional<PointCoordType> y, std::optional<Point> ij) :
		m_x { x }, m_y { y }, m_ij { ij } {
	// Empty
}

CoordinateData::~CoordinateData() {
}

CoordinateData CoordinateData::FromString(const std::string &str) {
	// Parse [X int][Y int][I int J int]
	DataTypeParser parser;
	std::string int_re = parser.GetPattern("integer");
	std::ostringstream pattern;
	pattern << "(X(" << int_re << "))?";
	pattern << "(Y(" << int_re << "))?";
	pattern << "(I(" << int_re << ")J(" << int_re << "))?";

	std::regex regex(pattern.str());
	std::smatch match;
	std::regex_search(str, match, regex);

	std::optional<PointCoordType> x, y;
	std::optional<Point> ij;
	// X-value
	if (match[2].matched) {
		x = parser.Integer(match[2].str());
	}

	// Y-value
	if (match[4].matched) {
		y = parser.Integer(match[4].str());
	}

	// IJ-value
	if (match[5].matched) {
		PointCoordType i = parser.Integer(match[6].str());
		PointCoordType j = parser.Integer(match[7].str());
		ij = Point(i, j);
	}

	return CoordinateData(x, y, ij);
}

std::optional<Point> CoordinateData::GetXY(const std::optional<Point> &defaultPt) const {
	if (!HasXY() && !defaultPt.has_value()) {
		return std::nullopt;
	}

	PointCoordType newX = GetX().value_or(defaultPt->GetX());
	PointCoordType newY = GetY().value_or(defaultPt->GetY());

	return Point(newX, newY);
}

bool CoordinateData::HasXY() const {
	return m_x.has_value() && m_y.has_value();
}

bool CoordinateData::HasIJ() const {
	return m_ij.has_value();
}

const std::optional<Point>& CoordinateData::GetIJ() const {
	return m_ij;
}

const std::optional<PointCoordType>& CoordinateData::GetX() const {
	return m_x;
}

const std::optional<PointCoordType>& CoordinateData::GetY() const {
	return m_y;
}