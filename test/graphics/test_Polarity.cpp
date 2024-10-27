/*
 * test_Polarity.cpp
 *
 *  Created on: Oct. 27, 2024
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
#include "Polarity.h"
#include "CppUTest/TestHarness.h"

namespace gerbex {

TEST_GROUP(Polarity) {

};

TEST(Polarity, PolarityFromCommand) {
	CHECK(Polarity::Clear == Polarity::FromCommand("C"));
	CHECK(Polarity::Dark == Polarity::FromCommand("D"));
	CHECK_THROWS(std::invalid_argument, Polarity::FromCommand("Q"));
}

} /* namespace gerbex */
