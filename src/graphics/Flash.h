/*
 * Flash.h
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

#ifndef FLASH_H_
#define FLASH_H_

#include "GraphicalObject.h"
#include "Serializable.h"

namespace gerbex {

/*
 * Replication of an aperture in the image plane.
 */
class Flash: public GraphicalObject {
public:
	Flash();
	Flash(const Point &origin,
			std::shared_ptr<Aperture> aperture, const ApertureTransformation &transformation);
	virtual ~Flash();
	void Serialize(Serializer &serializer) override;
};

} /* namespace gerbex */

#endif /* FLASH_H_ */
