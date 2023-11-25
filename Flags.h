#pragma once

#include <optional>
#include <string>

#include "PatternType.h"

namespace voronoi {

struct Flags {
	int width;
	int height;
	std::string outputFile;
	int numCells;
  PatternType patternType;
  bool tile;
};

std::optional<Flags> parseFlags(int argc, const char** argv);

} // namespace voronoi
