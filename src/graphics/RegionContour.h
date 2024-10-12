/*
 * RegionContour.h
 *
 *  Created on: Mar. 29, 2024
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

#ifndef REGIONCONTOUR_H_
#define REGIONCONTOUR_H_

#include "Segment.h"
#include "Serializer.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * A sequence of connected linear or circular segments.
 * Each segment must start where the previous ends.
 * Valid contours are closed, where the end point of the last segment coincides
 * with the start point of the first segment.
 */
class RegionContour: public Serializable {
public:
	RegionContour();
	virtual ~RegionContour();
	bool IsClosed() const;
	void AddSegment(std::shared_ptr<Segment> object);
	const std::vector<std::shared_ptr<Segment>>& GetSegments() const;
	void Serialize(Serializer &serializer) override;

private:
	std::vector<std::shared_ptr<Segment>> m_segments;

};

} /* namespace gerbex */

#endif /* REGIONCONTOUR_H_ */
