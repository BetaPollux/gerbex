/*
 * GraphicalObject.h
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

#ifndef GRAPHICALOBJECT_H_
#define GRAPHICALOBJECT_H_

#include "Box.h"
#include "Point.h"
#include <memory>

namespace gerbex {

class Serializer;

/*
 * Represents a plane figure that can be serialized.
 */
class GraphicalObject {
public:
	virtual ~GraphicalObject() = default;
	virtual void Translate(const Point &offset) = 0;
	virtual void ApplyTransform(const Transform &transform) = 0;
	virtual std::unique_ptr<GraphicalObject> Clone() = 0;
	virtual void Serialize(Serializer &serializer,
			const Point &origin) const = 0;
	virtual Box GetBox() const = 0;
};

} /* namespace gerbex */

#endif /* GRAPHICALOBJECT_H_ */
