#include <fstream>
#include <iostream>

#include "Flags.h"
#include "Image.h"
#include "VoronoiPattern.h"

using namespace voronoi;

int main(int argc, const char** argv) {
  std::optional<Flags> m_flags = parseFlags(argc, argv);

  if (m_flags.has_value()) {
    const Flags& flags = *m_flags;

    Image image =
        generateVoronoiPattern(flags.width, flags.height, flags.numCells, flags.patternType);

    std::ofstream ostream(flags.outputFile, std::ios::out | std::ios::binary);
    image.writeBitmapDataToStream(ostream);

    std::cout << "Done" << std::endl;
  }

	return 0;
}
