/*
 * CommandParser.h
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

#ifndef COMMANDPARSER_H_
#define COMMANDPARSER_H_

#include <string>
#include "CommandsProcessor.h"

typedef void (*commandHandler)(CommandsProcessor &, const std::vector<std::string> &);

/*
 *
 */
class CommandParser {
public:
	static std::string GetCommandCode(const std::string &word);
	static void AssertWordCommand(const std::vector<std::string> &words);
	static void AssertCode(const std::string &code, const std::string &expected);
	static void NotImplemented(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleComment(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleUnit(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleFormat(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleApertureDefine(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleApertureMacro(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleSetCurrentAperture(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandlePlotState(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandlePlot(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleMove(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleFlash(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleApertureTransformations(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleRegionStatement(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleBlockAperture(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleStepAndRepeat(CommandsProcessor &processor, const std::vector<std::string> &words);
	static void HandleEndOfFile(CommandsProcessor &processor, const std::vector<std::string> &words);
};

#endif /* COMMANDPARSER_H_ */
