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

#include "ArcSegment.h"
#include "Contour.h"
#include "SvgSerializer.h"
#include <fstream>
#include <iostream>
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

SvgSerializer::SvgSerializer(const Box &viewBox, double scaling) {
	m_svg = m_doc.append_child("svg");
	m_svg.append_attribute("xmlns") = "http://www.w3.org/2000/svg";
	m_defs = m_svg.append_child("defs");
	m_fgColor = "black";
	m_maskCounter = 0;
	m_lastGroup = pugi::xml_node();
	m_lastMask = pugi::xml_node();
	m_scaling = scaling;
	m_viewBox = scaleBox(viewBox);
	m_polarity = Polarity::Dark;
}

void SvgSerializer::SetViewPort(int width, int height) {
	m_svg.append_attribute("width") = width;
	m_svg.append_attribute("height") = height;
}

void SvgSerializer::setViewBox(const FixedBox &box) {
	std::stringstream box_stream;
	box_stream << box.GetLeft() << " ";
	box_stream << box.GetBottom() << " ";
	box_stream << box.GetWidth() << " ";
	box_stream << box.GetHeight();
	m_svg.remove_attribute("viewBox");
	m_svg.append_attribute("viewBox") = box_stream.str().c_str();
}

void SvgSerializer::SaveFile(const std::string &path) {
	setViewBox(m_viewBox);
	m_doc.save_file(path.c_str());
}

std::string SvgSerializer::makePathArc(const ArcSegment &segment) {
	//TODO solve for arc flags
	FixedPointType radius = scaleValue(
			segment.GetStart().Distance(segment.GetCenter()));
	FixedPoint e = scalePoint(segment.GetEnd());
	double rot = 0.0;
	int large_arc_flag = 0;
	int sweep_flag =
			segment.GetDirection() == ArcDirection::CounterClockwise ? 0 : 1;
	std::stringstream d;
	d << "A " << radius << " " << radius << " ";
	d << rot << " " << large_arc_flag << " " << sweep_flag << " ";
	d << e.GetX() << " " << e.GetY() << " ";
	return d.str();
}

std::string SvgSerializer::makePathLine(const Segment &segment) {
	FixedPoint e = scalePoint(segment.GetEnd());
	std::stringstream d;
	d << "L " << e.GetX() << " " << e.GetY() << " ";
	return d.str();
}

void SvgSerializer::SetForeground(const std::string &color) {
	m_fgColor = color;
}

void SvgSerializer::SetBackground(const std::string &color) {
	m_svg.remove_attribute("style");
	m_svg.append_attribute("style") = ("background-color:" + color).c_str();
}

pSerialItem SvgSerializer::NewGroup(pSerialItem target) {
	pugi::xml_node node = SvgItem::GetNode(target);
	pugi::xml_node group = node.append_child("g");
	return std::make_shared<SvgItem>(group);
}

pugi::xml_node SvgSerializer::newGlobalGroup() {
	pugi::xml_node group = m_svg.append_child("g");
	group.append_attribute("fill") = m_fgColor.c_str();
	group.append_attribute("stroke") = m_fgColor.c_str();
	group.append_attribute("stroke-width") = 0;
	return group;
}

pugi::xml_node SvgSerializer::newMask(pugi::xml_node parent,
		const FixedBox &box) {
	pugi::xml_node mask = parent.append_child("mask");
	std::string maskId = "mask" + std::to_string(m_maskCounter);
	m_maskCounter++;
	mask.append_attribute("id") = maskId.c_str();
	pugi::xml_node rect = mask.append_child("rect");
	setBox(rect, box);
	rect.append_attribute("fill") = "white";
	return mask;
}

pSerialItem SvgSerializer::NewMask(const Box &box) {
	const char *groupName = "macro-masks";
	pugi::xml_node maskGroup = m_defs.child(groupName);
	if (maskGroup.empty()) {
		maskGroup = m_defs.append_child(groupName);
	}
	pugi::xml_node mask = newMask(maskGroup, scaleBox(box));
	return std::make_shared<SvgItem>(mask);
}

pugi::xml_node SvgSerializer::newGlobalMask(const FixedBox &box) {
	pugi::xml_node mask = newMask(m_defs, box);

	std::string id = mask.attribute("id").as_string();
	std::string maskObjectsId = id + "-objects";
	pugi::xml_node maskObjects = m_defs.append_child("g");
	maskObjects.append_attribute("id") = maskObjectsId.c_str();

	std::string hrefAttr = "#" + maskObjectsId;

	for (pugi::xml_node def = m_defs.child("mask"); def;
			def = def.next_sibling("mask")) {
		pugi::xml_node use = def.append_child("use");
		use.append_attribute("href") = hrefAttr.c_str();
	}

	return maskObjects;
}

void SvgSerializer::AddArc(pSerialItem target, double width,
		const ArcSegment &segment) {
	pugi::xml_node node = SvgItem::GetNode(target);
	if (segment.IsCircle()) {
		FixedPoint c = scalePoint(segment.GetCenter());
		pugi::xml_node circle = node.append_child("circle");
		circle.append_attribute("r") = scaleValue(segment.GetRadius());
		circle.append_attribute("cx") = c.GetX();
		circle.append_attribute("cy") = c.GetY();
		circle.append_attribute("fill") = "none";
		circle.append_attribute("stroke-width") = scaleValue(width);
	} else {
		FixedPoint s = scalePoint(segment.GetStart());
		std::stringstream d;
		d << "M " << s.GetX() << " " << s.GetY() << " ";
		d << makePathArc(segment);
		pugi::xml_node path = node.append_child("path");
		path.append_attribute("d") = d.str().c_str();
		path.append_attribute("fill") = "none";
		path.append_attribute("stroke-width") = scaleValue(width);
		path.append_attribute("stroke-linecap") = "round";
	}
}

void SvgSerializer::AddCircle(pSerialItem target, double radius,
		const Point &center) {
	pugi::xml_node node = SvgItem::GetNode(target);
	pugi::xml_node circle = node.append_child("circle");
	FixedPoint c = scalePoint(center);
	circle.append_attribute("r") = scaleValue(radius);
	circle.append_attribute("cx") = c.GetX();
	circle.append_attribute("cy") = c.GetY();
}

void SvgSerializer::AddContour(pSerialItem target, const Contour &contour) {
	if (!contour.IsCircle()) {
		pugi::xml_node node = SvgItem::GetNode(target);
		const std::vector<std::shared_ptr<Segment>> segments =
				contour.GetSegments();

		FixedPoint s = scalePoint(segments[0]->GetStart());
		std::stringstream d;
		d << "M " << s.GetX() << " " << s.GetY() << " ";
		for (std::shared_ptr<Segment> segment : segments) {
			std::shared_ptr<ArcSegment> arc = std::dynamic_pointer_cast<
					ArcSegment>(segment);
			if (arc) {
				d << makePathArc(*arc);
			} else {
				d << makePathLine(*segment);
			}
		}
		pugi::xml_node path = node.append_child("path");
		path.append_attribute("d") = d.str().c_str();
	} else {
		const std::shared_ptr<ArcSegment> arc = std::dynamic_pointer_cast<
				ArcSegment>(contour.GetSegments().back());
		AddCircle(target, arc->GetRadius(), arc->GetCenter());
	}
}

void SvgSerializer::AddDraw(pSerialItem target, double width,
		const Segment &segment) {
	pugi::xml_node node = SvgItem::GetNode(target);
	FixedPoint s = scalePoint(segment.GetStart());
	FixedPoint e = scalePoint(segment.GetEnd());
	pugi::xml_node line = node.append_child("line");
	line.append_attribute("stroke-linecap") = "round";
	line.append_attribute("stroke-width") =
			std::to_string(scaleValue(width)).c_str();
	line.append_attribute("x1") = s.GetX();
	line.append_attribute("y1") = s.GetY();
	line.append_attribute("x2") = e.GetX();
	line.append_attribute("y2") = e.GetY();
}

void SvgSerializer::AddPolygon(pSerialItem target,
		const std::vector<Point> &points) {
	pugi::xml_node node = SvgItem::GetNode(target);
	pugi::xml_node poly = node.append_child("polygon");
	std::stringstream pts_stream;
	for (const Point &point : points) {
		FixedPoint p = scalePoint(point);
		pts_stream << p.GetX() << "," << p.GetY() << " ";
	}
	poly.append_attribute("points") = pts_stream.str().c_str();
}

void SvgSerializer::SetMask(pSerialItem target, pSerialItem mask) {
	pugi::xml_node target_node = SvgItem::GetNode(target);
	pugi::xml_node mask_node = SvgItem::GetNode(mask);
	setMask(target_node, mask_node);
}

void SvgSerializer::setMask(pugi::xml_node target, pugi::xml_node mask) const {
	std::string id = mask.attribute("id").as_string("missing_id");
	id = { id, 0, id.rfind("-objects") };
	std::string maskAttr = "url(#" + id + ")";
	target.remove_attribute("mask");
	target.append_attribute("mask") = maskAttr.c_str();
}

void SvgSerializer::setBox(pugi::xml_node node, const FixedBox &box) const {
	node.remove_attribute("x");
	node.remove_attribute("y");
	node.remove_attribute("width");
	node.remove_attribute("height");

	node.append_attribute("x") = box.GetLeft();
	node.append_attribute("y") = box.GetBottom();
	node.append_attribute("width") = box.GetWidth();
	node.append_attribute("height") = box.GetHeight();
}

pSerialItem SvgSerializer::GetTarget(Polarity polarity) {
	pugi::xml_node target;
	if (polarity == Polarity::Dark) {
		if (!m_lastGroup || m_polarity == Polarity::Clear) {
			m_lastGroup = newGlobalGroup();
		}
		target = m_lastGroup;
	} else {
		if (!m_lastMask || m_polarity == Polarity::Dark) {
			m_lastMask = newGlobalMask(m_viewBox);
			setMask(m_lastGroup, m_lastMask);
		}
		target = m_lastMask;
	}
	m_polarity = polarity;
	return std::make_shared<SvgItem>(target);
}

FixedPointType SvgSerializer::scaleValue(double value) const {
	return std::round(m_scaling * value);
}

FixedPoint SvgSerializer::scalePoint(const Point &point) const {
	return FixedPoint(std::round(m_scaling * point.GetX()),
			std::round(-m_scaling * point.GetY()));
}

FixedBox SvgSerializer::scaleBox(const Box &box) const {
	Box scaled = box * m_scaling;
	return FixedBox(std::round(scaled.GetWidth()),
			std::round(scaled.GetHeight()), std::round(scaled.GetLeft()),
			std::round(-scaled.GetTop()));
}

} /* namespace gerbex */
