/*
 * GraphicsStringFrom.cpp
 *
 *  Created on: Oct. 12, 2024
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

#include "GraphicsStringFrom.h"

SimpleString StringFrom(const gerbex::BlockAperture &block) {
	return StringFromFormat("%ld objects", block.GetObjectCount())
			+ StringFrom(", Box: ") + StringFrom(block.GetBox()) + ", T: "
			+ StringFrom(block.GetTransform());
}

SimpleString StringFrom(const gerbex::Box &box) {
	return StringFromFormat("%f, %f, (%f, %f)", box.GetWidth(), box.GetHeight(),
			box.GetLeft(), box.GetBottom());
}

SimpleString StringFrom(const gerbex::Circle &circle) {
	return StringFromFormat("%f, %f, T: ", circle.GetDiameter(),
			circle.GetHoleDiameter()) + StringFrom(circle.GetTransform());
}

SimpleString StringFrom(const gerbex::Point &point) {
	return StringFromFormat("(%f, %f)", point.GetX(), point.GetY());
}

SimpleString StringFrom(const gerbex::Transform &transform) {
	return StringFromFormat("%d, %d, %f, %f", (int) transform.GetPolarity(),
			(int) transform.GetMirroring(), transform.GetRotation(),
			transform.GetScaling());
}
