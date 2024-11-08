/*
 * CgalSerializer.cpp
 *
 *  Created on: Nov. 3, 2024
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
#include "Box.h"
#include "CgalSerializer.h"
#include "Contour.h"
#include "Point.h"
#include "Segment.h"
#include <cmath>
#include <CGAL/draw_polygon_set_2.h>

namespace gerbex {

//TODO this needs to do boolean ops on polygons before creating constraints

CgalSerializer::CgalSerializer() :
		m_polygonSet { std::make_shared<Polygon_set_2>() } {
}

pSerialItem CgalSerializer::NewMask(const Box &box) {
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::AddDraw(pSerialItem target, double width,
		const Segment &segment) {
	//TODO make draw
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::AddPolygon(pSerialItem target,
		const std::vector<Point> &points) {
	if (points.size() < 3) {
		throw std::invalid_argument("invalid polygon");
	}
	std::shared_ptr<Polygon_set_2> set = CgalItem::GetPolygonSet(target);
	Polygon_2 poly;
	for (const Point &pt : points) {
		poly.push_back(Point_2(pt.GetX(), pt.GetY()));
	}
	if (poly.orientation() == CGAL::Orientation::NEGATIVE) {
		poly.reverse_orientation();
	}
	set->join(poly);
	return std::make_shared<CgalItem>(poly);
}

pSerialItem CgalSerializer::NewGroup(pSerialItem parent) {
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::GetTarget(Polarity polarity) {
	return std::make_shared<CgalItem>(m_polygonSet);
}

pSerialItem CgalSerializer::AddArc(pSerialItem target, double width,
		const ArcSegment &segment) {
	//TODO make arc
	return std::make_shared<CgalItem>();
}

void CgalSerializer::SetMask(pSerialItem target, pSerialItem mask) {
}

pSerialItem CgalSerializer::AddCircle(pSerialItem target, double radius,
		const Point &center) {
	int numPoints = 12;
	double angleStep = 2 * M_PI / numPoints;
	std::vector<Point> points;
	for (int i = 0; i < numPoints; i++) {
		double angle = i * angleStep;
		double x = radius * cos(angle);
		double y = radius * sin(angle);
		points.push_back(center + Point(x, y));
	}
	return AddPolygon(target, points);
}

pSerialItem CgalSerializer::AddContour(pSerialItem target,
		const Contour &contour) {
	std::vector<Point> points;
	for (std::shared_ptr<Segment> seg : contour.GetSegments()) {
		//TODO handle ArcSegment
		points.push_back(seg->GetStart());
	}
	return AddPolygon(target, points);
}

void CgalSerializer::SaveFile(const std::string &path) {
	CGAL::draw(*m_polygonSet);
}

} /* namespace gerbex */
