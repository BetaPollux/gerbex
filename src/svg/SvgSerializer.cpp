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

//TODO need to fix y-axis

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

void SvgSerializer::AddDraw(double width, const Point &start,
		const Point &end) {
	Point s = start;
	Point e = end;
	s.Rotate(m_rotation);
	e.Rotate(m_rotation);
	s = s + m_offset;
	e = e + m_offset;
	pugi::xml_node line = m_svg.append_child("line");
	line.append_attribute("stroke") = "black";
	line.append_attribute("stroke-linecap") = "round";
	line.append_attribute("stroke-width") = std::to_string(width).c_str();
	line.append_attribute("x1") = s.GetX();
	line.append_attribute("y1") = s.GetY();
	line.append_attribute("x2") = e.GetX();
	line.append_attribute("y2") = e.GetY();
}

void SvgSerializer::AddArc(double width, const Point &start, const Point &end,
		const Point &center, ArcDirection direction) {
	AddDraw(width, center, start);
	AddDraw(width, center, end);
}

void SvgSerializer::AddCircle(double radius, const Point &center) {
	Point c = center;
	c.Rotate(m_rotation);
	c = c + m_offset;

	pugi::xml_node circle = m_svg.append_child("circle");
	circle.append_attribute("r") = radius;
	circle.append_attribute("cx") = c.GetX();
	circle.append_attribute("cy") = c.GetY();
}

void SvgSerializer::AddRectangle(double width, double height,
		const Point &topLeft) {
	if (m_rotation == 0.0) {
		Point pt = topLeft + m_offset;
		pugi::xml_node rect = m_svg.append_child("rect");
		rect.append_attribute("width") = width;
		rect.append_attribute("height") = height;
		rect.append_attribute("x") = pt.GetX();
		rect.append_attribute("y") = pt.GetY();
	} else {
		double dx = 0.5 * width;
		double dy = 0.5 * height;
		std::vector<Point> corners = { Point(dx, dy), Point(-dx, dy), Point(-dx,
				-dy), Point(dx, -dy) };
		AddPolygon(corners);
		// TODO investigate built-in SVG transforms
		//		std::string rot = "rotate(" + std::to_string(m_rotation) + ")";
		//		rect.append_attribute("transform") = rot.c_str();
		//		rect.append_attribute("transform-origin") = "center";
		//		rect.append_attribute("transform-box") = "fill-box";
	}
}

void SvgSerializer::AddPolygon(const std::vector<Point> &points) {
	pugi::xml_node poly = m_svg.append_child("polygon");
	std::stringstream pts_stream;
	for (auto pt : points) {
		pt.Rotate(m_rotation);
		pts_stream << pt.GetX() + m_offset.GetX() << ","
				<< pt.GetY() + m_offset.GetY() << " ";
	}
	poly.append_attribute("points") = pts_stream.str().c_str();
}

void SvgSerializer::AddObround(double width, double height,
		const Point &center) {
	if (width < height) {
		double w = width;
		double y = 0.5 * (height - width);
		AddDraw(w, Point(0.0, -y), Point(0.0, y));
	} else {
		double w = height;
		double x = 0.5 * (width - height);
		AddDraw(w, Point(-x, 0.0), Point(x, 0.0));
	}
}

} /* namespace gerbex */
