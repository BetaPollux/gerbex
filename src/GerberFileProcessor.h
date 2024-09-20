/*
 * GerberFileProcessor.h
 *
 *  Created on: Sep. 15, 2024
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

#ifndef GERBERFILEPROCESSOR_H_
#define GERBERFILEPROCESSOR_H_

#include "CommandParser.h"
#include "CommandsProcessor.h"
#include <memory>
#include <unordered_map>

/*
 *
 */
class GerberFileProcessor {
public:
	GerberFileProcessor();
	virtual ~GerberFileProcessor();
	void Process(std::unique_ptr<std::istream> stream);
	CommandsProcessor& GetProcessor();

private:
	CommandsProcessor m_processor;
	std::unordered_map<std::string, commandHandler> m_handlers;
};

#endif /* GERBERFILEPROCESSOR_H_ */
