/*
 * ObroundTemplate.h
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

#ifndef OBROUNDTEMPLATE_H_
#define OBROUNDTEMPLATE_H_

#include "ApertureTemplate.h"
#include "Obround.h"

namespace gerbex {

/*
 *
 */
class ObroundTemplate: public ApertureTemplate {
public:
	ObroundTemplate();
	virtual ~ObroundTemplate() = default;
	std::unique_ptr<Aperture> Call(const Parameters &parameters) override;
};

} /* namespace gerbex */

#endif /* OBROUNDTEMPLATE_H_ */
