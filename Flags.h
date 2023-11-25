#pragma once

#include <optional>
#include <string>

namespace voronoi {

struct Flags {
	int width;
	int height;
	std::string outputFile;
	int numCells;
};

std::optional<Flags> parseFlags(int argc, const char** argv);

} // namespace voronoi
