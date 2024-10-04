/*
 * Region.h
 *
 *  Created on: Mar. 24, 2024
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

#ifndef REGION_H_
#define REGION_H_

#include "RegionContour.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * Graphical object defined by a collection of contours.
 */
class Region: public GraphicalObject {
public:
	Region();
	Region(ApertureTransformation transformation);
	virtual ~Region();
	void StartContour();
	void AddSegment(std::shared_ptr<Segment> segment);
	const std::vector<RegionContour>& GetContours() const;
	bool AreContoursClosed() const;

private:
	std::vector<RegionContour> m_contours;

};

} /* namespace gerbex */

#endif /* REGION_H_ */
