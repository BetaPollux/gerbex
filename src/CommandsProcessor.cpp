/*
 * CommandsProcessor.cpp
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

#include "Circle.h"
#include "CommandsProcessor.h"
#include "Obround.h"
#include "Polygon.h"
#include "Rectangle.h"

CommandsProcessor::CommandsProcessor()
	: m_graphicsState{},
	  m_objects{},
	  m_apertures{},
	  m_templates{}
{
	m_templates["C"] = std::make_unique<Circle>();
	m_templates["R"] = std::make_unique<Rectangle>();
	m_templates["O"] = std::make_unique<Obround>();
	m_templates["P"] = std::make_unique<Polygon>();
}

CommandsProcessor::~CommandsProcessor() {
	m_apertures.clear();
	m_templates.clear();
}
