/*
 * MacroTemplate.cpp
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

#include "Macro.h"
#include "MacroTemplate.h"

namespace gerbex {

MacroTemplate::MacroTemplate() {
	// Empty

}

MacroTemplate::MacroTemplate(std::list<std::string> body)
	: m_body{ body }
{
	// Empty
}

MacroTemplate::~MacroTemplate() {
	// Empty
}


std::unique_ptr<Aperture> MacroTemplate::Call(const std::vector<double> &parameters) {
	//TODO create Macro
	//TODO insert parameters into body
	//TODO parse body
	//TODO add primitives
	return std::make_unique<Macro>();
}

const std::list<std::string>& MacroTemplate::GetBody() const {
	return m_body;
}

} /* namespace gerbex */
