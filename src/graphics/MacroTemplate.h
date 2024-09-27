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
	static MacroExposure ExposureFromNum(int num);

private:
	static std::unique_ptr<MacroPrimitive> HandleComment(const Parameters &params);
	static std::unique_ptr<MacroPrimitive> MakeCircle(const Parameters &params);
	static std::unique_ptr<MacroPrimitive> MakeCenterLine(const Parameters &params);
	static std::unique_ptr<MacroPrimitive> MakeVectorLine(const Parameters &params);
	static std::unique_ptr<MacroPrimitive> MakeOutline(const Parameters &params);
	static std::unique_ptr<MacroPrimitive> MakePolygon(const Parameters &params);
	static std::unique_ptr<MacroPrimitive> MakeThermal(const Parameters &params);

	Fields m_body;
};

} /* namespace gerbex */

#endif /* MACROTEMPLATE_H_ */
