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
#include "MacroPrimitive.h"
#include <string>
#include <unordered_map>

namespace gerbex {

enum class MacroCodes {
	COMMENT = 0,
	CIRCLE = 1,
	VECTOR_LINE = 20,
	CENTER_LINE = 21,
	OUTLINE = 4,
	POLYGON = 5,
	THERMAL = 7,
};

using Variables = std::unordered_map<int, double>;

/*
 * Creates a Macro aperture using parameters, variables and expressions.
 */
class MacroTemplate: public ApertureTemplate {
public:
	MacroTemplate();
	MacroTemplate(Fields body);
	virtual ~MacroTemplate();
	std::unique_ptr<Aperture> Call(const Parameters &parameters) override;
	const Fields &GetBody() const;

private:
	static Variables GetVariables(const Parameters &parameters);
	static void InsertVariables(std::string &block, const Variables &vars);
	static void DefineVariable(std::string &block, Variables &vars);

	Fields m_body;
};

} /* namespace gerbex */

#endif /* MACROTEMPLATE_H_ */
