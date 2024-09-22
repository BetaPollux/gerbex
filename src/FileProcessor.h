/*
 * FileProcessor.h
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

#ifndef FILEPROCESSOR_H_
#define FILEPROCESSOR_H_

#include "CommandsProcessor.h"
#include <memory>
#include <unordered_map>
#include "CommandHandler.h"

namespace gerbex {

/*
 *
 */
class FileProcessor {
public:
	FileProcessor();
	virtual ~FileProcessor();
	void Process(std::unique_ptr<std::istream> stream);
	CommandsProcessor& GetProcessor();

private:
	CommandsProcessor m_processor;
	std::unordered_map<std::string, callHandler> m_handlers;
};

} /* namespace gerbex */

#endif /* FILEPROCESSOR_H_ */
