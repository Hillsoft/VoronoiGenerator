#pragma once

#include <string>

namespace voronoi {

struct Flags {
	int width;
	int height;
	std::string outputFile;
	int numCells;
};

Flags parseFlags(int argc, const char** argv);

} // namespace voronoi
