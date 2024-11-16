/*
 * Contour.h
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

#ifndef CONTOUR_H_
#define CONTOUR_H_

#include "Segment.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * A sequence of connected linear or circular segments.
 * Each segment must start where the previous ends.
 * Valid contours are closed, where the end point of the last segment coincides
 * with the start point of the first segment.
 */
class Contour {
public:
	Contour();
	Contour(const Contour &rhs);
	virtual ~Contour() = default;
	bool operator==(const Contour &rhs) const;
	bool operator!=(const Contour &rhs) const;
	Contour& operator=(const Contour &rhs);
	bool IsClosed() const;
	bool IsCircle() const;
	void AddSegment(const std::shared_ptr<Segment> &segment);
	const std::vector<std::shared_ptr<Segment>>& GetSegments() const;
	void Translate(const Point &offset);
	void Transform(const gerbex::Transform &transform);

private:
	std::vector<std::shared_ptr<Segment>> m_segments;

};

} /* namespace gerbex */

#endif /* CONTOUR_H_ */
