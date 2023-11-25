#include <fstream>
#include <iostream>

#include "Flags.h"
#include "Image.h"
#include "VoronoiPattern.h"

using namespace voronoi;

int main(int argc, const char** argv) {
  Flags flags = parseFlags(argc, argv);

  Image image = generateVoronoiPattern(flags.width, flags.height, flags.numCells);

  std::ofstream ostream(flags.outputFile, std::ios::out | std::ios::binary);
  image.writeBitmapDataToStream(ostream);

	return 0;
}
