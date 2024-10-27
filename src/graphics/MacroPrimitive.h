/*
 * MacroPrimitive.h
 *
 *  Created on: Mar. 30, 2024
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

#ifndef MACROPRIMITIVE_H_
#define MACROPRIMITIVE_H_

#include "Box.h"
#include "Point.h"
#include "Transform.h"

namespace gerbex {

class Serializer;

enum class MacroExposure {
	OFF, ON
};

/*
 * A simple shape used to build a Macro aperture.
 * Primitives always rotate around the parent Macro origin.
 */
class MacroPrimitive {
public:
	MacroPrimitive() :
			MacroPrimitive(MacroExposure::ON) {
	}
	MacroPrimitive(MacroExposure exposure) :
			m_exposure { exposure } {
	}
	virtual ~MacroPrimitive() = default;
	MacroExposure GetExposure() const {
		return m_exposure;
	}
	static MacroExposure ExposureFromNum(int num) {
		switch (num) {
		case 1:
			return MacroExposure::ON;
		case 0:
			return MacroExposure::OFF;
		default:
			throw std::invalid_argument("macro exposure must be 0 or 1");
		}
	}
	//TODO needs new Serialize signature, macro does not use regular polarity
	virtual void Serialize(Serializer &serializer,
			const Point &origin) const = 0;
	virtual Box GetBox() const = 0;
	virtual void ApplyTransform(const Transform &transform) = 0;

protected:
	MacroExposure m_exposure;
};

} /* namespace gerbex */

#endif /* MACROPRIMITIVE_H_ */
