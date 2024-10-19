/*
 * MockAperture.cpp
 *
 *  Created on: Sep. 22, 2024
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

#include "MockAperture.h"

namespace gerbex {

MockAperture::MockAperture() {
	// Empty

}

MockAperture::~MockAperture() {
	// Empty
}

void MockAperture::Serialize(Serializer &serializer, const Point &origin,
		const ApertureTransformation &transform) {
	(void)serializer;
	(void)origin;
	(void)transform;
}

} /* namespace gerbex */
