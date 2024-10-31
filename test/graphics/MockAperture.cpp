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

#include "Box.h"
#include "MockAperture.h"
#include "CppUTestExt/MockSupport.h"

namespace gerbex {

MockAperture::MockAperture() :
		Circle() {
	// Empty
}

void MockAperture::Serialize(Serializer &serializer, pSerialItem target,
		const Point &origin) const {
	(void) serializer;
	(void) target;
	(void) origin;
	mock().actualCall("ApertureSerialize");
}

Box MockAperture::GetBox() const {
	return *(Box*) mock().actualCall("ApertureGetBox").returnPointerValue();
}

std::unique_ptr<Aperture> MockAperture::Clone() const {
	mock().actualCall("ApertureClone");
	return std::make_unique<MockAperture>();
}

void MockAperture::ApplyTransform(const gerbex::Transform &transform) {
	mock().actualCall("ApertureApplyTransform").withParameterOfType("Transform",
			"transform", &transform);
}

} /* namespace gerbex */
