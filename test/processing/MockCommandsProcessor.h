/*
 * MockCommandsProcessor.h
 *
 *  Created on: Sep. 22, 2024
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

#ifndef MOCKCOMMANDSPROCESSOR_H_
#define MOCKCOMMANDSPROCESSOR_H_

#include "CommandsProcessor.h"

namespace gerbex {

/*
 *
 */
class MockCommandsProcessor: public CommandsProcessor {
public:
	MockCommandsProcessor();
	virtual ~MockCommandsProcessor();
	void EndRegion() override;
	std::shared_ptr<ApertureTemplate> GetTemplate(std::string name) override;
	void OpenApertureBlock(int ident) override;
	void SetEndOfFile() override;
	const GraphicsState& GetGraphicsState() const override;
	void SetUnit(Unit unit) override;
	void PlotArc(const Point &coord, const Point &offset) override;
	CommandState GetCommandState() const override;
	void Flash(const Point &coord) override;
	const std::vector<std::shared_ptr<GraphicalObject>,
			std::allocator<std::shared_ptr<GraphicalObject>>>& GetObjects() const
			override;
	void CloseStepAndRepeat() override;
	void CloseApertureBlock() override;
	void SetFormat(const CoordinateFormat &format) override;
	void Move(const Point &coord) override;
	std::shared_ptr<Aperture> GetAperture(int ident) const override;
	void OpenStepAndRepeat(int nx, int ny, double dx, double dy) override;
	void StartRegion() override;
	void PlotDraw(const Point &coord) override;
	void ApertureDefine(int ident, std::shared_ptr<Aperture> aperture) override;
	void SetPlotState(PlotState state) override;
	void SetCurrentAperture(int ident) override;
	ApertureTransformation& GetTransformation() override;

private:
	ApertureTransformation m_apertureTransformation;
	GraphicsState m_graphicsState;
	CommandState m_commandState;
};

} /* namespace gerbex */

#endif /* MOCKCOMMANDSPROCESSOR_H_ */
