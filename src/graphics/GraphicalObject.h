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
#include "Serializable.h"

namespace gerbex {

/*
 * Represents a plane figure, with shape, size, position and polarity (dark/clear).
 */
class GraphicalObject: public Serializable {
public:
	virtual ~GraphicalObject() = default;
	virtual Box GetBox() const = 0;
};

} /* namespace gerbex */

#endif /* GRAPHICALOBJECT_H_ */
