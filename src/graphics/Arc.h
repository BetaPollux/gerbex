/*
 * Arc.h
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

#ifndef ARC_H_
#define ARC_H_

#include "Aperture.h"
#include "ApertureTransformation.h"
#include "ArcSegment.h"
#include "Circle.h"
#include "GraphicalObject.h"
#include <memory>

namespace gerbex {

/*
 * Circular segments drawn with a circle.
 */
class Arc: public GraphicalObject {
public:
	Arc();
	Arc(const ArcSegment &segment, std::shared_ptr<Circle> aperture,
			const ApertureTransformation &transformation);
	virtual ~Arc() = default;
	void Serialize(Serializer &serializer, const Point &origin,
			const ApertureTransformation &transform) override;
	std::shared_ptr<Circle> GetAperture() const;
	const ArcSegment& GetSegment() const;
	const ApertureTransformation& GetTransform() const;
	Box GetBox() const override;

private:
	ArcSegment m_segment;
	std::shared_ptr<Circle> m_aperture;
	ApertureTransformation m_transform;
};

} /* namespace gerbex */

#endif /* ARC_H_ */
