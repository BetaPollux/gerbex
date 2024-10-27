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

#include "Aperture.h"
#include "GraphicalObject.h"
#include "Point.h"
#include <memory>

namespace gerbex {

/*
 * Replication of an aperture in the image plane.
 */
class Flash: public GraphicalObject {
public:
	Flash();
	Flash(const Point &origin, std::shared_ptr<Aperture> aperture);
	virtual ~Flash() = default;
	void Serialize(Serializer &serializer, const Point &origin) const override;
	std::shared_ptr<Aperture> GetAperture() const;
	const Point& GetOrigin() const;
	Box GetBox() const override;
	void ApplyTransform(const Transform &transform) override;
	std::unique_ptr<GraphicalObject> Clone() override;
	void Translate(const Point &offset) override;

private:
	Point m_origin;
	std::shared_ptr<Aperture> m_aperture;
};

} /* namespace gerbex */

#endif /* FLASH_H_ */
