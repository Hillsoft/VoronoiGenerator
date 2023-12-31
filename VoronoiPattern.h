#pragma once

#include "Image.h"
#include "PatternType.h"

namespace voronoi {

Image generateVoronoiPattern(int width, int height, int numCells,
                             PatternType type, bool tile,
                             int distanceScaleFactor);

} // namespace voronoi
