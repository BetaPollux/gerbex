/*
 * gerbex.cpp
 *
 *  Created on: Oct. 11, 2024
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

#include "FileProcessor.h"
#include "SvgSerializer.h"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>

//TODO use an arg lib

using namespace gerbex;

int main(int argc, char *argv[]) {
	std::cout << "Gerbex" << std::endl;

	if (argc < 2) {
		std::cerr << "Usage: gerbex <gbr_file> [<svg_file>]" << std::endl;
		return EXIT_FAILURE;
	}

	std::filesystem::path gbr_file = argv[1];
	std::filesystem::path svg_file;
	if (argc > 2) {
		svg_file = argv[2];
	} else {
		svg_file = gbr_file.stem();
		svg_file += ".svg";
	}
	if (std::filesystem::exists(svg_file)) {
		std::cerr << "file already exists: " << svg_file << std::endl;
		return EXIT_FAILURE;
	}

	std::ifstream gerber = std::ifstream(gbr_file, std::ifstream::in);
	if (!gerber.good()) {
		std::cerr << "failed to open file" << std::endl;
		return EXIT_FAILURE;
	}
	FileProcessor fileProcessor;
	fileProcessor.Process(gerber);
	Box box = fileProcessor.GetProcessor().GetBox();
	std::cout << "Dimensions: " << box << std::endl;

	SvgSerializer serializer(box.Pad(0.5));
	serializer.SetViewPort(1000, 1000);
	serializer.SetForeground("red");
	serializer.SetBackground("black");

	std::vector<std::shared_ptr<GraphicalObject>> objects =
			fileProcessor.GetProcessor().GetObjects();

	for (std::shared_ptr<GraphicalObject> obj : objects) {
		obj->Serialize(serializer, Point());
	}

	serializer.SaveFile(svg_file);
}

