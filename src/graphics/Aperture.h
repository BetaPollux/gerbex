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

#include "Serializable.h"
#include "Transform.h"

namespace gerbex {

/*
 * An aperture is a 2D plane figure, and the basic tool to create graphic objects.
 * They can be rotated, mirrored or scaled.
 */
class Aperture: public Serializable {
public:
	Aperture() :
			m_transform { } {
	}
	virtual ~Aperture() = default;
	const Transform& GetTransform() const {
		return m_transform;
	}
	void SetTransform(const Transform &transform) {
		m_transform = transform;
	}

protected:
	Transform m_transform;
};

} /* namespace gerbex */

#endif /* APERTURE_H_ */
