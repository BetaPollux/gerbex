/*
 * Aperture.h
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

#ifndef APERTURE_H_
#define APERTURE_H_

#include "Box.h"
#include "Serializer.h"
#include <memory>

namespace gerbex {

/*
 * An aperture is a 2D plane figure, and the basic tool to create graphic objects.
 * They can be rotated, mirrored or scaled.
 */
class Aperture {
public:
	Aperture() = default;
	virtual ~Aperture() = default;
	virtual void Serialize(Serializer &serializer, pSerialItem target, const Point &origin) const = 0;
	virtual Box GetBox() const = 0;
	virtual std::unique_ptr<Aperture> Clone() const = 0;
	virtual void ApplyTransform(const Transform &transform) = 0;

};

} /* namespace gerbex */

#endif /* APERTURE_H_ */
