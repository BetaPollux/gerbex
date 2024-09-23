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
#include "ApertureTemplate.h"
#include "GraphicalObject.h"
#include "GraphicsState.h"
#include "Region.h"
#include "StepAndRepeat.h"
#include <unordered_map>
#include <memory>
#include <stack>
#include <string>
#include <vector>

namespace gerbex {

enum class CommandState {
	Normal,
	InsideRegion,
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
	virtual void ApertureDefine(int ident, std::shared_ptr<Aperture> aperture);
	virtual void OpenApertureBlock(int ident);
	virtual void CloseApertureBlock();
	virtual void SetCurrentAperture(int ident);
	virtual std::shared_ptr<Aperture> GetAperture(int ident) const;
	virtual void PlotDraw(const Point &coord);
	virtual void PlotArc(const Point &coord, const Point &offset);
	virtual void Move(const Point &coord);
	virtual void Flash(const Point &coord);
	virtual void StartRegion();
	virtual void EndRegion();
	virtual void AddTemplate(std::string name, std::shared_ptr<ApertureTemplate> new_tmpl);
	virtual std::shared_ptr<ApertureTemplate> GetTemplate(std::string name);
	virtual GraphicsState& GetGraphicsState();
	virtual const std::vector<std::shared_ptr<GraphicalObject>>& GetObjects() const;
	virtual CommandState GetCommandState() const;
	virtual void SetEndOfFile();
	virtual void OpenStepAndRepeat(int nx, int ny, double dx, double dy);
	virtual void CloseStepAndRepeat();

private:
	//TODO check for G75
	CommandState m_commandState;
	GraphicsState m_graphicsState;
	std::stack<std::vector<std::shared_ptr<GraphicalObject>>*> m_objectDest;
	std::vector<std::shared_ptr<GraphicalObject>> m_objects;
	std::unordered_map<int, std::shared_ptr<Aperture>> m_apertures;
	std::unordered_map<std::string, std::shared_ptr<ApertureTemplate>> m_templates;
	std::unique_ptr<Region> m_activeRegion;
	std::unique_ptr<StepAndRepeat> m_activeStepAndRepeat;
	int m_openBlocks;
};

} /* namespace gerbex */

#endif /* COMMANDSPROCESSOR_H_ */
