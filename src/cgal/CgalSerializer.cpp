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

#define NUM_ARC_POINTS 7
#define NUM_CIRCLE_POINTS 12
#define NUM_CAP_POINTS 5

namespace gerbex {

CgalSerializer::CgalSerializer() :
		m_items { } {
}

void CgalSerializer::AddDraw(pSerialItem target, double width,
		const Segment &segment) {
	std::shared_ptr<Polygon_set_2> set = CgalItem::GetPolygonSet(target);
	Point start = segment.GetStart();
	Point end = segment.GetEnd();
	double angle = atan2(end.GetY() - start.GetY(), end.GetX() - start.GetX());
	double radius = 0.5 * width;
	std::vector<Point_2> startCap = makeArc(start, radius, angle + M_PI_2,
			angle + 3.0 * M_PI_2, NUM_CAP_POINTS);
	std::vector<Point_2> endCap = makeArc(end, radius, angle - M_PI_2,
			angle + M_PI_2, NUM_CAP_POINTS);

	Polygon_2 poly;
	poly.insert(poly.end(), startCap.begin(), startCap.end());
	poly.insert(poly.end(), endCap.begin(), endCap.end());
	set->join(poly);
}

void CgalSerializer::AddPolygon(pSerialItem target,
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
}

pSerialItem CgalSerializer::NewGroup(pSerialItem parent) {
	std::shared_ptr<CgalItem> item = std::make_shared<CgalItem>(Polarity::Dark);
	m_items.push_back(item);
	return item;
}

pSerialItem CgalSerializer::NewMask(const Box &box) {
	return std::make_shared<CgalItem>(Polarity::Clear);
}

pSerialItem CgalSerializer::GetTarget(Polarity polarity) {
	std::shared_ptr<CgalItem> item = std::make_shared<CgalItem>(polarity);
	m_items.push_back(item);
	return item;
}

void CgalSerializer::AddArc(pSerialItem target, double width,
		const ArcSegment &segment) {
	std::shared_ptr<Polygon_set_2> set = CgalItem::GetPolygonSet(target);
	if (segment.IsCircle()) {
		Point c = segment.GetCenter();
		double r = segment.GetRadius();
		double dr = 0.5 * width;
		Polygon_2 outer = makeRegularPolygon(c, r + dr, NUM_CIRCLE_POINTS);
		Polygon_2 inner = makeRegularPolygon(c, r - dr, NUM_CIRCLE_POINTS);
		inner.reverse_orientation();
		Polygon_with_holes_2 circle(outer);
		circle.add_hole(inner);
		set->join(circle);
	} else {
		Point start = segment.GetStart();
		Point end = segment.GetEnd();
		Point center = segment.GetCenter();
		if (segment.GetDirection() == ArcDirection::Clockwise) {
			start = segment.GetEnd();
			end = segment.GetStart();
		}
		double startAngle = atan2(start.GetY() - center.GetY(),
				start.GetX() - center.GetX());
		double endAngle = atan2(end.GetY() - center.GetY(),
				end.GetX() - center.GetX());
		double capRadius = 0.5 * width;
		double outerRadius = segment.GetRadius() + capRadius;
		double innerRadius = segment.GetRadius() - capRadius;
		int arcN = NUM_ARC_POINTS;
		int capN = NUM_CAP_POINTS;
		std::vector<Point_2> startCap = makeArc(start, capRadius,
				startAngle - M_PI, startAngle, capN);
		startCap.pop_back();
		std::vector<Point_2> outerArc = makeArc(center, outerRadius, startAngle,
				endAngle, arcN);
		outerArc.pop_back();
		std::vector<Point_2> endCap = makeArc(end, capRadius, endAngle,
				endAngle + M_PI, capN);
		endCap.pop_back();
		std::vector<Point_2> innerArc = makeArc(center, innerRadius, endAngle,
				startAngle, arcN);
		innerArc.pop_back();

		Polygon_2 poly;
		poly.insert(poly.end(), startCap.begin(), startCap.end());
		poly.insert(poly.end(), outerArc.begin(), outerArc.end());
		poly.insert(poly.end(), endCap.begin(), endCap.end());
		poly.insert(poly.end(), innerArc.begin(), innerArc.end());
		set->join(poly);
	}
}

void CgalSerializer::SetMask(pSerialItem target, pSerialItem mask) {
	std::shared_ptr<Polygon_set_2> targetSet = CgalItem::GetPolygonSet(target);
	std::shared_ptr<Polygon_set_2> maskSet = CgalItem::GetPolygonSet(mask);
	targetSet->difference(*maskSet);
}

void CgalSerializer::AddCircle(pSerialItem target, double radius,
		const Point &center) {
	std::shared_ptr<Polygon_set_2> set = CgalItem::GetPolygonSet(target);
	Polygon_2 circle = makeRegularPolygon(center, radius, NUM_CIRCLE_POINTS);
	set->join(circle);
}

void CgalSerializer::AddContour(pSerialItem target, const Contour &contour) {
	if (!contour.IsCircle()) {
		std::shared_ptr<Polygon_set_2> set = CgalItem::GetPolygonSet(target);
		Polygon_2 poly;
		for (std::shared_ptr<Segment> seg : contour.GetSegments()) {
			std::shared_ptr<ArcSegment> arc = std::dynamic_pointer_cast<
					ArcSegment>(seg);
			if (arc) {
				Point start = arc->GetStart();
				Point end = arc->GetEnd();
				Point center = arc->GetCenter();
				double startAngle = atan2(start.GetY() - center.GetY(),
						start.GetX() - center.GetX());
				double endAngle = atan2(end.GetY() - center.GetY(),
						end.GetX() - center.GetX());
				if (arc->GetDirection() == ArcDirection::Clockwise
						&& endAngle > startAngle) {
					endAngle -= 2.0 * M_PI;
				} else if (arc->GetDirection() == ArcDirection::CounterClockwise
						&& endAngle < startAngle) {
					endAngle += 2.0 * M_PI;
				}
				std::vector<Point_2> arc_points = makeArc(center,
						arc->GetRadius(), startAngle, endAngle, NUM_ARC_POINTS);
				arc_points.pop_back();
				poly.insert(poly.end(), arc_points.begin(), arc_points.end());
			} else {
				Point_2 pt(seg->GetStart().GetX(), seg->GetStart().GetY());
				poly.push_back(pt);
			}
		}
		if (poly.orientation() == CGAL::Orientation::NEGATIVE) {
			poly.reverse_orientation();
		}
		set->join(poly);
	} else {
		const std::shared_ptr<ArcSegment> arc = std::dynamic_pointer_cast<
				ArcSegment>(contour.GetSegments().back());
		AddCircle(target, arc->GetRadius(), arc->GetCenter());
	}
}

void CgalSerializer::SaveFile(const std::string &path) {
	Polygon_set_2 result;
	for (std::shared_ptr<CgalItem> item : m_items) {
		if (item->GetPolarity() == Polarity::Dark) {
			result.join(*item->GetPolygonSet());
		} else {
			result.difference(*item->GetPolygonSet());
		}
	}
	CGAL::draw(result);
}

std::vector<Point_2> CgalSerializer::makeArc(const Point &center, double radius,
		double start, double end, int N) {
	double step = (end - start) / (N - 1);
	std::vector<Point_2> vertices;
	for (int i = 0; i < N; i++) {
		double angle = start + i * step;
		Point_2 pt(center.GetX() + radius * cos(angle),
				center.GetY() + radius * sin(angle));
		vertices.push_back(pt);
	}
	return vertices;
}

Polygon_2 CgalSerializer::makeRegularPolygon(const Point &center, double radius,
		int N) {
	double angleStep = 2 * M_PI / N;
	Polygon_2 poly;
	for (int i = 0; i < N; i++) {
		double angle = i * angleStep;
		double x = radius * cos(angle);
		double y = radius * sin(angle);
		Point_2 pt(x + center.GetX(), y + center.GetY());
		poly.push_back(pt);
	}
	return poly;
}

} /* namespace gerbex */
