/*
 * SvgSerializer.cpp
 *
 *  Created on: Oct. 9, 2024
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

#include "SvgSerializer.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace gerbex {

SvgSerializer::SvgSerializer() {
	m_svg = m_doc.append_child("svg");
	m_svg.append_attribute("xmlns") = "http://www.w3.org/2000/svg";
}

SvgSerializer::~SvgSerializer() {
	// Empty
}

void SvgSerializer::SetViewPort(int width, int height) {
	m_svg.append_attribute("width") = width;
	m_svg.append_attribute("height") = height;
}

void SvgSerializer::SetViewBox(double xMin, double yMin, double width,
		double height) {
	std::stringstream box_stream;
	box_stream << xMin << " ";
	box_stream << yMin << " ";
	box_stream << width << " ";
	box_stream << height;
	m_svg.append_attribute("viewBox") = box_stream.str().c_str();
}

void SvgSerializer::SaveFile(const std::string &path) {
	m_doc.save_file(path.c_str());
}

void SvgSerializer::AddCircle(double radius, double centerX, double centerY) {
	pugi::xml_node circle = m_svg.append_child("circle");
	circle.append_attribute("r") = radius;
	circle.append_attribute("cx") = centerX;
	circle.append_attribute("cy") = centerY;
}

void SvgSerializer::AddRectangle(double width, double height, double left,
		double top) {
	pugi::xml_node rect = m_svg.append_child("circle");
	rect.append_attribute("width") = width;
	rect.append_attribute("height") = height;
	rect.append_attribute("x") = left;
	rect.append_attribute("y") = top;
}

void SvgSerializer::AddPolygon(
		const std::vector<std::pair<double, double> > &points) {
	pugi::xml_node poly = m_svg.append_child("polygon");
	std::stringstream pts_stream;
	for (auto pt: points) {
		pts_stream << pt.first << "," << pt.second << " ";
	}
	poly.append_attribute("points") = pts_stream.str().c_str();
}

} /* namespace gerbex */
