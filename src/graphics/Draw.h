/*
 * Draw.h
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

#ifndef DRAW_H_
#define DRAW_H_

#include "Circle.h"
#include "GraphicalObject.h"
#include "Segment.h"
#include <memory>

namespace gerbex {

/*
 * Straight line segments drawn with a circle.
 */
class Draw: public GraphicalObject {
public:
	Draw();
	Draw(const Segment &segment, std::shared_ptr<Aperture> aperture);
	virtual ~Draw() = default;
	void Serialize(Serializer &serializer, const Point &origin) const override;
	double GetDrawWidth() const;
	const Segment& GetSegment() const;
	Box GetBox() const override;
	void ApplyTransform(const Transform &transform) override;
	std::unique_ptr<GraphicalObject> Clone() override;
	void Translate(const Point &offset) override;

private:
	Segment m_segment;
	double m_drawWidth;
};

} /* namespace gerbex */

#endif /* DRAW_H_ */
