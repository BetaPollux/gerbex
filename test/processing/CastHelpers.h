/*
 * CastHelpers.h
 *
 *  Created on: Oct. 3, 2024
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

#ifndef CASTHELPERS_H_
#define CASTHELPERS_H_

#include "CommandsProcessor.h"
#include "Flash.h"
#include "Macro.h"
#include <memory>
#include "CppUTest/TestHarness.h"

namespace gerbex {

/**
 * Helper functions
 */

template <typename T> std::shared_ptr<T> MakeAndSetAperture(CommandsProcessor &processor, int id) {
	std::shared_ptr<T> ap = std::make_shared<T>();

	processor.ApertureDefine(id, ap);
	processor.SetCurrentAperture(id);

	return ap;
}

template <typename T> std::shared_ptr<T> GetGraphicalObject(const std::vector<std::shared_ptr<GraphicalObject>> &objs, size_t idx = 0) {
	CHECK(objs.size() > idx);

	std::shared_ptr<GraphicalObject> obj = objs[idx];
	std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(obj);

	CHECK_TEXT(result != nullptr, "graphical object was wrong type");

	return result;
}

template <typename T> std::shared_ptr<T> CheckAperture(const Flash &flash) {
	std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(flash.GetAperture());

	CHECK_TEXT(result != nullptr, "flash aperture was wrong type");

	return result;
}

template <typename T> std::shared_ptr<T> GetAperture(CommandsProcessor &processor, int ident) {
	std::shared_ptr<Aperture> obj = processor.GetAperture(ident);
	std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(obj);

	CHECK_TEXT(result != nullptr, "aperture dictionary entry was wrong type");

	return result;
}

template <typename T> std::shared_ptr<T> GetMacroPrimitive(Macro &macro, size_t idx = 0) {
	CHECK(macro.GetPrimitives().size() > idx);

	std::shared_ptr<MacroPrimitive> obj = macro.GetPrimitives()[idx];
	std::shared_ptr<T> result = std::dynamic_pointer_cast<T>(obj);

	CHECK_TEXT(result != nullptr, "macro primitive was wrong type");

	return result;
}

} /* namespace gerbex */

#endif /* CASTHELPERS_H_ */
