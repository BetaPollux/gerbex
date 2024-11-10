/*
 * CgalSerializer.h
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
#ifndef CGALSERIALIZER_H_
#define CGALSERIALIZER_H_

#include "Serializer.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polygon_with_holes_2.h>
#include <CGAL/Polygon_set_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon_2;
typedef CGAL::Polygon_with_holes_2<K> Polygon_with_holes_2;
typedef CGAL::Polygon_set_2<K> Polygon_set_2;
typedef CGAL::Point_2<K> Point_2;

namespace gerbex {

class CgalItem: public SerialItem {
public:
	CgalItem() :
			m_polygonSet { std::make_shared<Polygon_set_2>() } {
	}
	CgalItem(std::shared_ptr<Polygon_set_2> polygonSet) :
			m_polygonSet { polygonSet } {
	}
	CgalItem(const Polygon_2 &polygon) :
			CgalItem() {
		m_polygonSet->insert(polygon);
	}
	virtual ~CgalItem() = default;
	const std::shared_ptr<Polygon_set_2> GetPolygonSet() const {
		return m_polygonSet;
	}
	static std::shared_ptr<Polygon_set_2> GetPolygonSet(pSerialItem item) {
		std::shared_ptr<CgalItem> cgal = std::dynamic_pointer_cast<CgalItem>(
				item);
		if (!cgal) {
			throw std::invalid_argument("Svg received non-Svg item");
		}
		return cgal->m_polygonSet;
	}

private:
	std::shared_ptr<Polygon_set_2> m_polygonSet;
};

class CgalSerializer: public Serializer {
public:
	CgalSerializer();
	virtual ~CgalSerializer() = default;
	pSerialItem NewMask(const Box &box) override;
	pSerialItem AddDraw(pSerialItem target, double width,
			const Segment &segment) override;
	pSerialItem AddPolygon(pSerialItem target, const std::vector<Point> &points)
			override;
	pSerialItem NewGroup(pSerialItem parent) override;
	pSerialItem GetTarget(Polarity polarity) override;
	pSerialItem AddArc(pSerialItem target, double width,
			const ArcSegment &segment) override;
	void SetMask(pSerialItem target, pSerialItem mask) override;
	pSerialItem AddCircle(pSerialItem target, double radius,
			const Point &center) override;
	pSerialItem AddContour(pSerialItem target, const Contour &contour) override;
	void SaveFile(const std::string &path) override;

private:
	std::vector<Point_2> makeArc(const Point &center, double radius, double start,
			double end, int N);
	Polygon_2 makeRegularPolygon(const Point &center, double radius, int N);
	std::shared_ptr<Polygon_set_2> m_polygonSet;
};

} /* namespace gerbex */

#endif /* CGALSERIALIZER_H_ */
