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

namespace gerbex {

//TODO this needs to do boolean ops on polygons before creating constraints

pSerialItem CgalSerializer::NewMask(const Box &box) {
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::AddDraw(pSerialItem target, double width,
		const Segment &segment) {
	addLineSegment(m_cdt, segment.GetStart(), segment.GetEnd());
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::AddPolygon(pSerialItem target,
		const std::vector<Point> &points) {
	if (points.size() < 3) {
		throw std::invalid_argument("invalid polygon");
	}
	for (size_t i = 1; i < points.size(); i++) {
		addLineSegment(m_cdt, points[i - 1], points[i]);
	}
	addLineSegment(m_cdt, points.back(), points.front());
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::NewGroup(pSerialItem parent) {
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::GetTarget(Polarity polarity) {
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::AddArc(pSerialItem target, double width,
		const ArcSegment &segment) {
	addLineSegment(m_cdt, segment.GetStart(), segment.GetEnd());
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
	AddPolygon(target, points);
	return std::make_shared<CgalItem>();
}

pSerialItem CgalSerializer::AddContour(pSerialItem target,
		const Contour &contour) {
	for (std::shared_ptr<Segment> seg : contour.GetSegments()) {
		addLineSegment(m_cdt, seg->GetStart(), seg->GetEnd());
	}
	return std::make_shared<CgalItem>();
}

void CgalSerializer::SaveFile(const std::string &path) {
	std::ofstream file(path);
	CGAL::IO::write_VTU(file, m_cdt);
	file.close();
}

void CgalSerializer::addLineSegment(CDT &cdt, const Point &pt1,
		const Point &pt2) {
	CDT::Vertex_handle va = cdt.insert(CDT::Point(pt1.GetX(), pt1.GetY()));
	CDT::Vertex_handle vb = cdt.insert(CDT::Point(pt2.GetX(), pt2.GetY()));
	cdt.insert_constraint(va, vb);
}

} /* namespace gerbex */
