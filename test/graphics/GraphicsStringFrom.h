/*
 * StringFrom.h
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

#ifndef GRAPHICSSTRINGFROM_H_
#define GRAPHICSSTRINGFROM_H_

#include "BlockAperture.h"
#include "Box.h"
#include "Circle.h"
#include "Point.h"
#include "Transform.h"
#include "CppUTest/SimpleString.h"

SimpleString StringFrom(const gerbex::BlockAperture &block);
SimpleString StringFrom(const gerbex::Box &box);
SimpleString StringFrom(const gerbex::Circle &circle);
SimpleString StringFrom(const gerbex::Point &point);
SimpleString StringFrom(const gerbex::Transform &transform);

#endif /* GRAPHICSSTRINGFROM_H_ */
