/*
 * CommandsProcessor.h
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

#ifndef COMMANDSPROCESSOR_H_
#define COMMANDSPROCESSOR_H_

#include "Aperture.h"
#include "GraphicalObject.h"
#include "GraphicsState.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

/*
 *	Processes commands as provided by the syntax parser.
 *	Maintains collection of all graphics objects
 */
class CommandsProcessor {
public:
	CommandsProcessor();
	virtual ~CommandsProcessor();

private:
	GraphicsState m_graphicsState;
	std::vector<GraphicalObject> m_objects;
	std::map<int, std::unique_ptr<Aperture>> m_apertures;
	std::map<std::string, std::unique_ptr<Aperture>> m_templates;
};

#endif /* COMMANDSPROCESSOR_H_ */
