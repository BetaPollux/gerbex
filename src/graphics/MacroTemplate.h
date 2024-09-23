/*
 * MacroTemplate.h
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

#ifndef MACROTEMPLATE_H_
#define MACROTEMPLATE_H_

#include "ApertureTemplate.h"
#include <string>
#include <vector>

namespace gerbex {

/*
 * Creates a Macro aperture using parameters, variables and expressions.
 */
class MacroTemplate: public ApertureTemplate {
public:
	MacroTemplate();
	MacroTemplate(std::vector<std::string> body);
	virtual ~MacroTemplate();
	std::unique_ptr<Aperture> Call(const std::vector<double> &parameters) override;
	const std::vector<std::string> &GetBody() const;

private:
	std::vector<std::string> m_body;
};

} /* namespace gerbex */

#endif /* MACROTEMPLATE_H_ */
