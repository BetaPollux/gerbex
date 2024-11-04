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

#include "CgalSerializer.h"
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

enum class GerbexMode {
	Svg, Cgal
};

int main(int argc, char *argv[]) {
	std::cout << "Gerbex" << std::endl;

	if (argc < 3) {
		std::cerr << "Usage: gerbex svg|cgal <gbr_file> [<out_file>]"
				<< std::endl;
		return EXIT_FAILURE;
	}

	GerbexMode mode;
	std::string modeStr = argv[1];
	std::string fileExt;
	if (modeStr == "svg") {
		mode = GerbexMode::Svg;
		fileExt = ".svg";
	} else if (modeStr == "cgal") {
		mode = GerbexMode::Cgal;
		fileExt = ".vtu";
	} else {
		std::cerr << "unrecognized mode " << modeStr << std::endl;
		return EXIT_FAILURE;
	}

	std::filesystem::path gbr_file = argv[2];
	std::filesystem::path out_file;
	if (argc > 3) {
		out_file = argv[3];
	} else {
		out_file = gbr_file.stem();
		out_file += fileExt;
	}

	if (std::filesystem::exists(out_file)) {
		std::cerr << "output file already exists: " << out_file << std::endl;
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

	std::unique_ptr<Serializer> serializer;
	switch (mode) {
	case GerbexMode::Svg: {
		std::unique_ptr<SvgSerializer> svgSerializer = std::make_unique<
				SvgSerializer>(box.Pad(0.5));
		svgSerializer->SetViewPort(1000, 1000);
		svgSerializer->SetForeground("red");
		svgSerializer->SetBackground("black");
		serializer = std::move(svgSerializer);
		break;
	}
	case GerbexMode::Cgal: {
		std::unique_ptr<CgalSerializer> cgalSerializer = std::make_unique<
				CgalSerializer>();
		serializer = std::move(cgalSerializer);
		break;
	}
	default:
		std::cerr << "unrecognized mode" << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<std::shared_ptr<GraphicalObject>> objects =
			fileProcessor.GetProcessor().GetObjects();

	for (std::shared_ptr<GraphicalObject> obj : objects) {
		obj->Serialize(*serializer, Point());
	}

	serializer->SaveFile(out_file);
}

