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

#include <string>
#include "CommandsProcessor.h"

namespace gerbex {

typedef void (*callHandler)(CommandsProcessor&, Fields&);

/*
 *
 */
class CommandHandler {
public:
	static void AssertWordCommand(const Fields &words);
	static void AssertCommandCode(const std::string &code,
			const std::string &expected);
	static void NotImplemented(CommandsProcessor &processor, Fields &words);
	static void Comment(CommandsProcessor &processor, Fields &words);
	static void Unit(CommandsProcessor &processor, Fields &words);
	static void Format(CommandsProcessor &processor, Fields &words);
	static void ApertureDefine(CommandsProcessor &processor, Fields &words);
	static void ApertureMacro(CommandsProcessor &processor, Fields &words);
	static void SetCurrentAperture(CommandsProcessor &processor, Fields &words);
	static void PlotState(CommandsProcessor &processor, Fields &words);
	static void Plot(CommandsProcessor &processor, Fields &words);
	static void Move(CommandsProcessor &processor, Fields &words);
	static void Flash(CommandsProcessor &processor, Fields &words);
	static void ApertureTransformations(CommandsProcessor &processor,
			Fields &words);
	static void RegionStatement(CommandsProcessor &processor, Fields &words);
	static void BlockAperture(CommandsProcessor &processor, Fields &words);
	static void StepAndRepeat(CommandsProcessor &processor, Fields &words);
	static void EndOfFile(CommandsProcessor &processor, Fields &words);
};

} /* namespace gerbex */

#endif /* COMMANDHANDLER_H_ */
