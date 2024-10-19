/*
 * Macro.h
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

#ifndef MACRO_H_
#define MACRO_H_

#include "Aperture.h"
#include "MacroPrimitive.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * A compound aperture made of primitives.
 */
class Macro: public Aperture {
public:
	Macro();
	virtual ~Macro();
	void AddPrimitive(std::shared_ptr<MacroPrimitive> primitive);
	const std::vector<std::shared_ptr<MacroPrimitive>>& GetPrimitives() const;
	void Serialize(Serializer &serializer, const Point &origin,
			const ApertureTransformation &transform) override;
	Box GetBox() const override;

private:
	std::vector<std::shared_ptr<MacroPrimitive>> m_primitives;

};

} /* namespace gerbex */

#endif /* MACRO_H_ */
