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

	std::string path = argv[1];
	std::string svg_file = argc > 2 ? argv[2] : "gerber.svg";

	std::unique_ptr<std::istream> gerber = std::make_unique<std::ifstream>(path,
			std::ifstream::in);
	FileProcessor fileProcessor;
	fileProcessor.Process(std::move(gerber));

	SvgSerializer serializer;
	serializer.SetViewPort(400, 400);
	//TODO set viewbox to match extents
	serializer.SetViewBox(-1.0, -1.0, 7.0, 7.0);

	std::vector<std::shared_ptr<GraphicalObject>> objects =
			fileProcessor.GetProcessor().GetObjects();

	for (std::shared_ptr<GraphicalObject> obj: objects) {
		obj->Serialize(serializer);
	}

	serializer.SaveFile(svg_file);
}

