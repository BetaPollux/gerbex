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

enum class CommandState {
	Normal,
	InsideMacro,
	InsideRegion,
	InsideBlock,
	InsideStepAndRepeat,
	EndOfFile
};

/*
 *	Processes commands as provided by the syntax parser.
 *	Maintains collection of all graphics objects
 */
class CommandsProcessor {
public:
	CommandsProcessor();
	virtual ~CommandsProcessor();
	void ApertureDefinition(int ident, std::shared_ptr<Aperture> aperture);
	void SetCurrentAperture(int ident);
	void SetPlotState(PlotState state);
	void PlotDraw(const Point &coord);
	void PlotArc(const Point &coord, const Point &offset);
	void Move(const Point &coord);
	void Flash(const Point &coord);
	std::shared_ptr<Aperture> GetTemplate(std::string name);
	const GraphicsState& GetGraphicsState() const;
	const std::vector<std::shared_ptr<GraphicalObject> >& GetObjects() const;
	CommandState GetCommandState() const;
	void SetCommandState(CommandState commandState);

private:
	//TODO check for G75, check if currentpoint was set
	CommandState m_commandState;
	GraphicsState m_graphicsState;
	std::vector<std::shared_ptr<GraphicalObject>> m_objects;
	std::map<int, std::shared_ptr<Aperture>> m_apertures;
	std::map<std::string, std::shared_ptr<Aperture>> m_templates;
};

#endif /* COMMANDSPROCESSOR_H_ */
