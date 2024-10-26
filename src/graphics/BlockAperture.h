/*
 * BlockAperture.h
 *
 *  Created on: Mar. 29, 2024
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

#ifndef BLOCKAPERTURE_H_
#define BLOCKAPERTURE_H_

#include "Aperture.h"
#include "Transformable.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 *
 */
class BlockAperture: public Aperture {
public:
	BlockAperture() = default;
	virtual ~BlockAperture() = default;
	bool operator==(const BlockAperture &rhs) const;
	bool operator!=(const BlockAperture &rhs) const;
	void AddObject(std::shared_ptr<Transformable> object);
	std::vector<std::shared_ptr<Transformable>> *GetObjectList();
	size_t GetObjectCount() const;
	void Serialize(Serializer &serializer, const Point &origin) const override;
	Box GetBox() const override;
	std::unique_ptr<Aperture> Clone() const override;
	void SetTransform(const Transform &transform) override;

private:
	std::vector<std::shared_ptr<Transformable>> m_objects;
};

} /* namespace gerbex */

#endif /* BLOCKAPERTURE_H_ */
