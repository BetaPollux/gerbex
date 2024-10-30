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

// SVG Y-axis has 0 at the top, whereas Gerber has 0 at the bottom.
// This class negates all Y-coords to compensate.
// This is also results in the arc CW vs CCW being reversed.
//	Gerber 	-> SVG
//		left 	-> min x
//		right 	-> max x
//		bottom 	-> max y
//		top 	-> -1 * min y

SvgSerializer::SvgSerializer(const Box &viewBox) {
	m_svg = m_doc.append_child("svg");
	m_svg.append_attribute("xmlns") = "http://www.w3.org/2000/svg";
	m_defs = m_svg.append_child("defs");
	m_fgColor = "black";
	m_bgColor = "";
	m_polarity = Polarity::Dark();
	m_maskCounter = 0;
	m_lastGroup = pugi::xml_node();
	m_lastMask = pugi::xml_node();
	setViewBox(viewBox);
}

void SvgSerializer::SetViewPort(int width, int height) {
	m_svg.append_attribute("width") = width;
	m_svg.append_attribute("height") = height;
}

void SvgSerializer::setViewBox(const Box &box) {
	std::stringstream box_stream;
	box_stream << box.GetLeft() << " ";
	box_stream << -box.GetTop() << " ";
	box_stream << box.GetWidth() << " ";
	box_stream << box.GetHeight();
	m_svg.remove_attribute("viewBox");
	m_svg.append_attribute("viewBox") = box_stream.str().c_str();
}

void SvgSerializer::SaveFile(const std::string &path) {
	if (m_bgColor != "") {
		m_svg.append_attribute("style") = ("background-color:" + m_bgColor).c_str();
	}
	m_doc.save_file(path.c_str());
}

void SvgSerializer::AddDraw(double width, const Segment &segment) {
	Point s = segment.GetStart();
	Point e = segment.GetEnd();
	pugi::xml_node line = m_svg.append_child("line");
	line.append_attribute("stroke") = getFillColor();
	line.append_attribute("stroke-linecap") = "round";
	line.append_attribute("stroke-width") = std::to_string(width).c_str();
	line.append_attribute("x1") = s.GetX();
	line.append_attribute("y1") = -s.GetY();
	line.append_attribute("x2") = e.GetX();
	line.append_attribute("y2") = -e.GetY();
}

void SvgSerializer::AddArc(double width, const ArcSegment &segment) {
	if (segment.IsCircle()) {
		Point c = segment.GetCenter();
		pugi::xml_node circle = m_svg.append_child("circle");
		circle.append_attribute("r") = segment.GetRadius();
		circle.append_attribute("cx") = c.GetX();
		circle.append_attribute("cy") = -c.GetY();
		circle.append_attribute("fill") = "none";
		circle.append_attribute("stroke") = getFillColor();
		circle.append_attribute("stroke-width") = width;
	} else {
		Point s = segment.GetStart();
		std::stringstream d;
		d << "M " << s.GetX() << " " << -s.GetY() << " ";
		d << makePathArc(segment);
		pugi::xml_node path = m_svg.append_child("path");
		path.append_attribute("d") = d.str().c_str();
		path.append_attribute("fill") = "none";
		path.append_attribute("stroke") = getFillColor();
		path.append_attribute("stroke-width") = width;
		path.append_attribute("stroke-linecap") = "round";
	}
}

void SvgSerializer::AddCircle(double radius, const Point &center) {
	pugi::xml_node circle = m_svg.append_child("circle");
	circle.append_attribute("r") = radius;
	circle.append_attribute("cx") = center.GetX();
	circle.append_attribute("cy") = -center.GetY();
	circle.append_attribute("fill") = getFillColor();
}

void SvgSerializer::AddPolygon(const std::vector<Point> &points) {
	pugi::xml_node poly = m_svg.append_child("polygon");
	std::stringstream pts_stream;
	for (auto pt : points) {
		pts_stream << pt.GetX() << "," << -pt.GetY() << " ";
	}
	poly.append_attribute("points") = pts_stream.str().c_str();
	poly.append_attribute("fill") = getFillColor();
}

const char* SvgSerializer::getFillColor() const {
	if (m_polarity == Polarity::Dark()) {
		return m_fgColor.c_str();
	} else {
		return m_bgColor.c_str();
	}
}

void SvgSerializer::AddContour(const Contour &contour) {
	const std::vector<std::shared_ptr<Segment>> segments =
			contour.GetSegments();
	Point s = segments[0]->GetStart();
	std::stringstream d;
	d << "M " << s.GetX() << " " << -s.GetY() << " ";
	for (std::shared_ptr<Segment> segment : segments) {
		std::shared_ptr<ArcSegment> arc = std::dynamic_pointer_cast<ArcSegment>(
				segment);
		if (arc) {
			d << makePathArc(*arc);
		} else {
			d << makePathLine(*segment);
		}
	}
	pugi::xml_node path = m_svg.append_child("path");
	path.append_attribute("d") = d.str().c_str();
	path.append_attribute("fill") = getFillColor();
}

std::string SvgSerializer::makePathArc(const ArcSegment &segment) {
	//TODO solve for arc flags
	double radius = segment.GetStart().Distance(segment.GetCenter());
	Point e = segment.GetEnd();
	double rot = 0.0;
	int large_arc_flag = 0;
	int sweep_flag =
			segment.GetDirection() == ArcDirection::CounterClockwise ? 0 : 1;
	std::stringstream d;
	d << "A " << radius << " " << radius << " ";
	d << rot << " " << large_arc_flag << " " << sweep_flag << " ";
	d << e.GetX() << " " << -e.GetY() << " ";
	return d.str();
}

std::string SvgSerializer::makePathLine(const Segment &segment) {
	Point e = segment.GetEnd();
	std::stringstream d;
	d << "L " << e.GetX() << " " << -e.GetY() << " ";
	return d.str();
}

void SvgSerializer::SetForeground(const std::string &color) {
	m_fgColor = color;
}

void SvgSerializer::SetBackground(const std::string &color) {
	m_bgColor = color;
}

pugi::xml_node SvgSerializer::GetLastGroup() {
	if (m_lastGroup.empty()) {
		m_lastGroup = NewGroup();
	}
	return m_lastGroup;
}

pugi::xml_node SvgSerializer::GetLastMask(const Box &box) {
	if (m_lastMask.empty()) {
		m_lastMask = NewMask(box);
	}
	return m_lastMask;
}

pugi::xml_node SvgSerializer::NewGroup() {
	pugi::xml_node group = m_svg.append_child("g");
	group.append_attribute("fill") = m_fgColor.c_str();
	group.append_attribute("stroke") = m_fgColor.c_str();
	group.append_attribute("stroke-width") = 0;
	return group;
}

pugi::xml_node SvgSerializer::NewMask(const Box &box) {
	pugi::xml_node mask = m_defs.append_child("mask");
	std::string id = "mask" + std::to_string(m_maskCounter);
	m_maskCounter++;
	mask.append_attribute("id") = id.c_str();
	pugi::xml_node rect = mask.append_child("rect");
	setBox(rect, box);
	rect.append_attribute("fill") = "white";
	return mask;
}

pugi::xml_node SvgSerializer::AddCircle(pugi::xml_node target, double radius,
		const Point &center) {
	pugi::xml_node circle = target.append_child("circle");
	circle.append_attribute("r") = radius;
	circle.append_attribute("cx") = center.GetX();
	circle.append_attribute("cy") = -center.GetY();
	return circle;
}

void SvgSerializer::SetMask(pugi::xml_node target, pugi::xml_node mask) {
	std::string id = mask.attribute("id").as_string("missing_id");
	std::string maskAttr = "url(#" + id + ")";
	target.remove_attribute("mask");
	target.append_attribute("mask") = maskAttr.c_str();
}

void SvgSerializer::setBox(pugi::xml_node node, const Box &box) {
	node.remove_attribute("x");
	node.remove_attribute("y");
	node.remove_attribute("width");
	node.remove_attribute("height");

	node.append_attribute("x") = box.GetLeft();
	node.append_attribute("y") = -box.GetTop();
	node.append_attribute("width") = box.GetWidth();
	node.append_attribute("height") = box.GetHeight();
}

} /* namespace gerbex */
