/*
 * Transformable.h
 *
 *  Created on: Oct. 25, 2024
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
#ifndef TRANSFORMABLE_H_
#define TRANSFORMABLE_H_

#include "GraphicalObject.h"
//TODO flatten back with graphical object if StepAndRepeat is removed

namespace gerbex {

class Transform;

class Transformable : public GraphicalObject {
public:
	Transformable() = default;
	virtual ~Transformable() = default;
	virtual void ApplyTransform(const Transform &transform) = 0;
	virtual std::unique_ptr<Transformable> Clone() = 0;
};

} /* namespace gerbex */

#endif /* TRANSFORMABLE_H_ */
