/*
 * StepAndRepeat.h
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

#ifndef STEPANDREPEAT_H_
#define STEPANDREPEAT_H_

#include "GraphicalObject.h"
#include <memory>
#include <vector>

namespace gerbex {

/*
 * Takes an object list and duplicates them in the desired amount.
 * Each replication is at an dx, dy offset.
 * Copies are first in positive Y then positive X direction.
 */
class StepAndRepeat: public GraphicalObject {
public:
	StepAndRepeat();
	StepAndRepeat(int nx, int ny, double dx, double dy);
	virtual ~StepAndRepeat() = default;
	std::vector<std::shared_ptr<GraphicalObject>> *GetObjectList();
	void AddObject(std::shared_ptr<GraphicalObject> object);
	double GetDx() const;
	double GetDy() const;
	int GetNx() const;
	int GetNy() const;
	void Serialize(Serializer &serializer, const Point &origin,
			const Transform &transform) const override;
	Box GetBox() const override;

private:
	std::vector<std::shared_ptr<GraphicalObject>> m_objects;
	int m_nx, m_ny;
	double m_dx, m_dy;
};

} /* namespace gerbex */

#endif /* STEPANDREPEAT_H_ */
