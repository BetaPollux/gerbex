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
class SvgSerializer : public Serializer {
public:
	SvgSerializer();
	virtual ~SvgSerializer();
	void SetViewPort(int width, int height);
	void SetViewBox(double xMin, double yMin, double width, double height);
	void SaveFile(const std::string &path);
	void AddCircle(double radius, double centerX, double centerY) override;
	void AddRectangle(double width, double height, double left, double top) override;
	void AddPolygon(const std::vector<std::pair<double, double>> &points) override;
	void AddDraw(double width, double x0, double y0, double x1, double y1)
			override;

private:
	pugi::xml_document m_doc;
	pugi::xml_node m_svg;
};

} /* namespace gerbex */

#endif /* SVGSERIALIZER_H_ */
