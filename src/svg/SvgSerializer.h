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

//TODO scale numbers to be reasonable integers?

class SvgItem: public SerialItem {
public:
	SvgItem() :
			m_node { } {
	}
	SvgItem(pugi::xml_node node) :
			m_node { node } {
	}
	virtual ~SvgItem() = default;
	pugi::xml_node GetNode() {
		return m_node;
	}
	static pugi::xml_node GetNode(pSerialItem item) {
		std::shared_ptr<SvgItem> svg = std::dynamic_pointer_cast<SvgItem>(item);
		if (!svg) {
			throw std::invalid_argument("Svg received non-Svg item");
		}
		return svg->m_node;
	}

private:
	pugi::xml_node m_node;
};

/*
 *
 */
class SvgSerializer: public Serializer {
public:
	SvgSerializer(const Box &viewBox);
	virtual ~SvgSerializer() = default;
	void SetViewPort(int width, int height);
	void SaveFile(const std::string &path);
	void SetForeground(const std::string &color);
	void SetBackground(const std::string &color);
	pSerialItem NewGroup() override;
	pSerialItem NewMask(const Box &box) override;
	void SetMask(pSerialItem target, pSerialItem mask) override;
	pSerialItem AddArc(pSerialItem target, double width,
			const ArcSegment &segment) override;
	pSerialItem AddCircle(pSerialItem target, double radius,
			const Point &center) override;
	pSerialItem AddContour(pSerialItem target, const Contour &contour) override;
	pSerialItem AddDraw(pSerialItem target, double width,
			const Segment &segment) override;
	pSerialItem AddPolygon(pSerialItem target, const std::vector<Point> &points)
			override;
	pSerialItem GetTarget(Polarity polarity) override;

private:
	void setViewBox(const Box &box);
	std::string makePathArc(const ArcSegment &segment);
	std::string makePathLine(const Segment &segment);
	void setBox(pugi::xml_node node, const Box &box);
	pugi::xml_document m_doc;
	pugi::xml_node m_svg;
	pugi::xml_node m_defs;
	std::string m_fgColor;
	std::string m_bgColor;
	int m_maskCounter;
	Box m_viewBox;
	pSerialItem m_lastGroup;
	pSerialItem m_lastMask;
	Polarity m_polarity;
};

} /* namespace gerbex */

#endif /* SVGSERIALIZER_H_ */
