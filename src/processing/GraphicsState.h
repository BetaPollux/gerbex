/*
 * GraphicsState.h
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

#ifndef GRAPHICSSTATE_H_
#define GRAPHICSSTATE_H_

#include "Aperture.h"
#include "CoordinateData.h"
#include "CoordinateFormat.h"
#include "Point.h"
#include "Transform.h"
#include <memory>
#include <optional>

namespace gerbex {

enum class Unit {
	Millimeter,
	Inch
};


enum class PlotState {
	Linear,
	Clockwise,
	CounterClockwise
};

enum class ArcMode {
	SingleQuadrant,
	MultiQuadrant
};

/*
 * Graphics state is a set of parameters affecting the result of operation codes.
 */
class GraphicsState {
public:
	GraphicsState();
	virtual ~GraphicsState();
	const std::shared_ptr<Aperture> GetCurrentAperture() const;
	void SetCurrentAperture(std::shared_ptr<Aperture> aperture);
	void SetCurrentPoint(std::optional<Point> point);
	std::optional<Point> GetCurrentPoint() const;
	std::optional<CoordinateFormat> GetFormat() const;
	void SetFormat(std::optional<CoordinateFormat> format);
	std::optional<PlotState> GetPlotState() const;
	void SetPlotState(std::optional<PlotState> plotState);
	Transform& GetTransformation();
	void SetTransformation(const Transform &transformation);
	std::optional<Unit> GetUnit() const;
	void SetUnit(std::optional<Unit> unit);
	static Unit UnitFromCommand(const std::string &str);
	static PlotState PlotStateFromCommand(const std::string &str);
	static ArcMode ArcModeFromCommand(const std::string &str);
	std::optional<ArcMode> GetArcMode() const;
	void SetArcMode(std::optional<ArcMode> arcMode);
	Point GetPoint(const CoordinateData &data) const;
	void AssertPlotState();
	void AssertArcMode();

private:
	std::optional<CoordinateFormat> m_format;
	std::optional<Unit> m_unit;
	std::optional<Point> m_currentPoint;
	std::shared_ptr<Aperture> m_currentAperture;
	std::optional<PlotState> m_plotState;
	std::optional<ArcMode> m_arcMode;
	Transform m_transformation;
};

} /* namespace gerbex */

#endif /* GRAPHICSSTATE_H_ */
