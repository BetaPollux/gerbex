/*
 * CommandHandler.h
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

#ifndef COMMANDHANDLER_H_
#define COMMANDHANDLER_H_

#include <list>
#include <string>
#include "CommandsProcessor.h"

namespace gerbex {

typedef void (*callHandler)(CommandsProcessor &, std::list<std::string> &);

/*
 *
 */
class CommandHandler {
public:
	static void AssertWordCommand(const std::list<std::string> &words);
	static void AssertCommandCode(const std::string &code, const std::string &expected);
	static void NotImplemented(CommandsProcessor &processor, std::list<std::string> &words);
	static void Comment(CommandsProcessor &processor, std::list<std::string> &words);
	static void Unit(CommandsProcessor &processor, std::list<std::string> &words);
	static void Format(CommandsProcessor &processor, std::list<std::string> &words);
	static void ApertureDefine(CommandsProcessor &processor, std::list<std::string> &words);
	static void ApertureMacro(CommandsProcessor &processor, std::list<std::string> &words);
	static void SetCurrentAperture(CommandsProcessor &processor, std::list<std::string> &words);
	static void PlotState(CommandsProcessor &processor, std::list<std::string> &words);
	static void Plot(CommandsProcessor &processor, std::list<std::string> &words);
	static void Move(CommandsProcessor &processor, std::list<std::string> &words);
	static void Flash(CommandsProcessor &processor, std::list<std::string> &words);
	static void ApertureTransformations(CommandsProcessor &processor, std::list<std::string> &words);
	static void RegionStatement(CommandsProcessor &processor, std::list<std::string> &words);
	static void BlockAperture(CommandsProcessor &processor, std::list<std::string> &words);
	static void StepAndRepeat(CommandsProcessor &processor, std::list<std::string> &words);
	static void EndOfFile(CommandsProcessor &processor, std::list<std::string> &words);
};

} /* namespace gerbex */

#endif /* COMMANDHANDLER_H_ */
