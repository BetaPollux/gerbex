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
	virtual ~SvgSerializer();
	void SetViewPort(int width, int height);
	void SetViewBox(double xMin, double yMin, double width, double height);
	void SaveFile(const std::string &path);
	void AddDraw(double width, const Segment &segment, bool isDark = true)
			override;
	void AddArc(double width, const ArcSegment &segment, bool isDark = true)
			override;
	void AddCircle(double radius, const Point &center, bool isDark = true)
			override;
	void AddPolygon(const std::vector<Point> &points, bool isDark = true)
			override;
	void AddObround(double width, double height, const Point &center,
			bool isDark = true) override;
	void AddContour(const std::vector<std::shared_ptr<Segment>> &segments,
			bool isDark = true) override;

private:
	std::string makePathArc(const ArcSegment &segment);
	std::string makePathLine(const Segment &segment);
	const char* getFillColour(bool isDark) const;
	pugi::xml_document m_doc;
	pugi::xml_node m_svg;
};

} /* namespace gerbex */

#endif /* SVGSERIALIZER_H_ */
