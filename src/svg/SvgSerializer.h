/*
 * SvgSerializer.h
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

#ifndef SVGSERIALIZER_H_
#define SVGSERIALIZER_H_

#include "Box.h"
#include "GraphicalObject.h"
#include "Serializer.h"
#include <string>
#include <vector>
#include <pugixml.hpp>

namespace gerbex {

/*
 *
 */
class SvgSerializer: public Serializer {
public:
	SvgSerializer();
	virtual ~SvgSerializer() = default;
	void SetViewPort(int width, int height);
	void SetViewBox(const Box &box);
	void SaveFile(const std::string &path);
	void SetForeground(const std::string &color);
	void SetBackground(const std::string &color);
	void AddDraw(double width, const Segment &segment) override;
	void AddArc(double width, const ArcSegment &segment) override;
	void AddCircle(double radius, const Point &center) override;
	void AddPolygon(const std::vector<Point> &points) override;
	void AddContour(const Contour &contour) override;
	pugi::xml_node MakeGroup();
	pugi::xml_node MakeMask(const Box& box);
	pugi::xml_node AddCircle(pugi::xml_node target, double radius, const Point &center);
	void ApplyMask(pugi::xml_node target, pugi::xml_node mask);
//TODO these should all return an element that can be modified with more calls
//TODO CreateMask, create <mask> and add shape to <mask> vs image
//TODO ApplyMask, adds mask= attribute

private:
	std::string makePathArc(const ArcSegment &segment);
	std::string makePathLine(const Segment &segment);
	const char* getFillColor() const;
	pugi::xml_document m_doc;
	pugi::xml_node m_svg;
	pugi::xml_node m_defs;
	std::string m_fgColor;
	std::string m_bgColor;
	int m_maskCounter;
};

} /* namespace gerbex */

#endif /* SVGSERIALIZER_H_ */
