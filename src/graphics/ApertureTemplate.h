/*
 * ApertureTemplate.h
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

#ifndef APERTURETEMPLATE_H_
#define APERTURETEMPLATE_H_

#include "Aperture.h"
#include "Circle.h"
#include "DataTypeParser.h"
#include <memory>
#include <vector>

namespace gerbex {


/*
 * An Aperture Template is used to create Apertures
 */
class ApertureTemplate {
public:
	virtual ~ApertureTemplate();
	virtual std::unique_ptr<Aperture> Call(const Parameters &parameters) = 0;
};

} /* namespace gerbex */

#endif /* APERTURETEMPLATE_H_ */
