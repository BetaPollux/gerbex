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
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Delaunay_mesh_face_base_2.h>
#include <CGAL/Triangulation_conformer_2.h>
#include <CGAL/IO/write_VTU.h>
#include <iostream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Delaunay_mesh_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, Tds> CDT;

namespace gerbex {

class CgalItem: public SerialItem {
public:
	CgalItem() = default;
	virtual ~CgalItem() = default;
};

class CgalSerializer: public Serializer {
public:
	CgalSerializer() = default;
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
	void addLineSegment(CDT &cdt, const Point &pt1, const Point &pt2);
	CDT m_cdt;
};

} /* namespace gerbex */

#endif /* CGALSERIALIZER_H_ */
