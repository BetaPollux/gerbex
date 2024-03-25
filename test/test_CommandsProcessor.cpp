/*
 * test_CommandsProcessor.cpp
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

#include "CommandsProcessor.h"
#include "Circle.h"
#include <stdexcept>
#include "CppUTest/TestHarness.h"

TEST_GROUP(CommandsProcessor_Init) {
	CommandsProcessor processor;
};

TEST(CommandsProcessor_Init, State) {
	LONGS_EQUAL(CommandState::Normal, processor.GetCommandState());
}

TEST(CommandsProcessor_Init, StandardTemplates) {
	CHECK(processor.GetTemplate("C") != nullptr);
	CHECK(processor.GetTemplate("R") != nullptr);
	CHECK(processor.GetTemplate("O") != nullptr);
	CHECK(processor.GetTemplate("P") != nullptr);
}

TEST(CommandsProcessor_Init, GetTemplate_DoesNotExist) {
	CHECK_THROWS(std::invalid_argument, processor.GetTemplate("X"));
}

TEST(CommandsProcessor_Init, SetCurrentAperture_DoesNotExist) {
	CHECK_THROWS(std::invalid_argument, processor.SetCurrentAperture(10));
}

TEST(CommandsProcessor_Init, ApertureDefinition) {
	std::shared_ptr<Circle> circle = std::make_unique<Circle>();

	processor.ApertureDefinition(10, circle);
	processor.SetCurrentAperture(10);

	POINTERS_EQUAL(circle.get(), processor.GetGraphicsState().GetCurrentAperture().get());
}

TEST(CommandsProcessor_Init, ApertureDefinition_BadNumber) {
	//0 through 9 are illegal
	std::unique_ptr<Circle> circle = std::make_unique<Circle>();

	CHECK_THROWS(std::invalid_argument, processor.ApertureDefinition(9, std::move(circle)));
}

TEST(CommandsProcessor_Init, ApertureDefinition_Null) {
	CHECK_THROWS(std::invalid_argument, processor.ApertureDefinition(10, nullptr));
}

