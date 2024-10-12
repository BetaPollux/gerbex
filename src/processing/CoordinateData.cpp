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

namespace gerbex {

CoordinateData::CoordinateData() :
		CoordinateData(0, 0) {
	// Empty
}

CoordinateData::CoordinateData(std::optional<FixedPointType> x,
		std::optional<FixedPointType> y, std::optional<FixedPoint> ij) :
		m_x { x }, m_y { y }, m_ij { ij } {
	// Empty
}

CoordinateData::~CoordinateData() {
}

CoordinateData CoordinateData::FromString(const std::string &str) {
	// Parse [X int][Y int][I int J int]

	std::string num_re = DataTypeParser::GetNumberPattern();
	std::ostringstream pattern;
	pattern << "(X(" << num_re << "))?";
	pattern << "(Y(" << num_re << "))?";
	pattern << "(I(" << num_re << ")J(" << num_re << "))?";

	std::regex regex(pattern.str());
	std::smatch match;
	std::regex_search(str, match, regex);

	std::optional<FixedPointType> x, y;
	std::optional<FixedPoint> ij;
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
		FixedPointType i = std::stoi(match[6].str());
		FixedPointType j = std::stoi(match[7].str());
		ij = FixedPoint(i, j);
	}

	return CoordinateData(x, y, ij);
}

bool CoordinateData::HasXY() const {
	return m_x.has_value() && m_y.has_value();
}

std::optional<FixedPoint> CoordinateData::GetIJ() const {
	return m_ij;
}

FixedPoint CoordinateData::GetIJChecked() const {
	if (!m_ij.has_value()) {
		throw std::invalid_argument("offset IJ was not defined");
	}
	return *m_ij;
}

std::optional<FixedPointType> CoordinateData::GetX() const {
	return m_x;
}

std::optional<FixedPointType> CoordinateData::GetY() const {
	return m_y;
}

void CoordinateData::AssertNoIJ() const {
	if (m_ij.has_value()) {
		throw std::invalid_argument("offset IJ defined but invalid for operation");
	}
}

} /* namespace gerbex */

