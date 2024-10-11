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

#include "SvgSerializer.h"
#include <fstream>
#include <vector>

#define DEFAULT_WIDTH 500
#define DEFAULT_HEIGHT 500

namespace gerbex {

template<typename T>
inline std::string attribute(std::string const &name, const T &value) {
	std::stringstream stream;
	stream << name << "=\"" << value << "\" ";
	return stream.str();
}

template<typename T>
inline std::string attribute(std::string const &name,
		const std::vector<T> &values) {
	std::stringstream stream;
	stream << name << "=\"";
	for (T val : values) {
		stream << val << " ";
	}
	stream << "\" ";
	return stream.str();
}

SvgSerializer::SvgSerializer() :
		m_viewPortWidth { DEFAULT_WIDTH }, m_viewPortHeight { DEFAULT_HEIGHT }, m_viewBoxXMin {
				0 }, m_viewBoxYMin { 0 }, m_viewBoxWidth {
		DEFAULT_WIDTH }, m_viewBoxHeight { DEFAULT_HEIGHT } {
	// Empty
}

SvgSerializer::~SvgSerializer() {
	// Empty
}

void SvgSerializer::SetViewPort(int width, int height) {
	m_viewPortWidth = width;
	m_viewPortHeight = height;
}

void SvgSerializer::SetViewBox(double xMin, double yMin, double width,
		double height) {
	m_viewBoxXMin = xMin;
	m_viewBoxYMin = yMin;
	m_viewBoxWidth = width;
	m_viewBoxHeight = height;
}

void SvgSerializer::SaveFile(const std::string &path) {
	//TODO most of these tags should be optional
	m_stream << "</svg>\n";
	std::ofstream file;
	file.open(path);
	file << "<?xml ";
	file << attribute("version", "1.0");
	file << attribute("encoding", "utf-8") << "?>\n";
	file << "<svg " << attribute("width", m_viewPortWidth);
	file << attribute("height", m_viewPortHeight);
	file << "xmlns=\"http://www.w3.org/2000/svg\" ";
	std::vector<double> viewBox = { m_viewBoxXMin, m_viewBoxYMin,
			m_viewBoxWidth, m_viewBoxHeight };
	file << attribute("viewBox", viewBox) << ">\n";
	file << m_stream.rdbuf();
	file.close();
}

void SvgSerializer::AddCircle(double radius, double centerX, double centerY) {
	m_stream << "<circle ";
	m_stream << attribute("r", radius);
	m_stream << attribute("cx", centerX);
	m_stream << attribute("cy", centerY);
	m_stream << "/>\n";
}

void SvgSerializer::AddRectangle(double width, double height, double left,
		double top) {
	m_stream << "<rect ";
	m_stream << attribute("width", width);
	m_stream << attribute("height", height);
	m_stream << attribute("x", left);
	m_stream << attribute("y", top);
	m_stream << "/>\n";
}

void SvgSerializer::AddPolygon(
		const std::vector<std::pair<double, double> > &points) {
	m_stream << "<poylgon ";
	std::stringstream pts_stream;
	for (auto pt: points) {
		pts_stream << pt.first << "," << pt.second << " ";
	}
	m_stream << attribute("points", pts_stream.str());
	m_stream << "/>\n";
}

} /* namespace gerbex */
