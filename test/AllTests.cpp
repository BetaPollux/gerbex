/*
 * AllTests.cpp
 *
 *  Created on: Dec. 4, 2022
 *	Copyright (C) 2022 BetaPollux
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

#include "Comparators.h"
#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/TestRegistry.h"
#include "CppUTestExt/MockSupportPlugin.h"

int main(int ac, char** av)
{
	PointComparator pointComparator;
	ParamsComparator paramsComparator;
	MacroTemplateComparator macroComparator;
	TransformComparator transformComparator;
	MockSupportPlugin mockPlugin;

	mockPlugin.installComparator("Point", pointComparator);
	mockPlugin.installComparator("Parameters", paramsComparator);
	mockPlugin.installComparator("MacroTemplate", macroComparator);
	mockPlugin.installComparator("Transform", transformComparator);
	TestRegistry::getCurrentRegistry()->installPlugin(&mockPlugin);

    return CommandLineTestRunner::RunAllTests(ac, av);
}
