/*
 * GraphicalObject.h
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

#ifndef GRAPHICALOBJECT_H_
#define GRAPHICALOBJECT_H_

#include "Aperture.h"
#include "ApertureTransformation.h"
#include "Point.h"

/*
 * Represents a plane figure, with shape, size, position and polarity (dark/clear).
 */
class GraphicalObject {
public:
	GraphicalObject();
	GraphicalObject(const Point &origin,
			Aperture *aperture, const ApertureTransformation &transformation);
	virtual ~GraphicalObject();

protected:
	Point m_origin;
	Aperture *m_aperture;
	ApertureTransformation m_transformation;
};

#endif /* GRAPHICALOBJECT_H_ */
