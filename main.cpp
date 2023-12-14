#include <chrono>
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

    const auto start = std::chrono::steady_clock::now();

    Image image = generateVoronoiPattern(flags.width, flags.height,
                                         flags.numCells, flags.patternType,
                                         flags.tile, flags.distanceScaleFactor);

    const auto imageComplete = std::chrono::steady_clock::now();

    {
      std::ofstream ostream(flags.outputFile, std::ios::out | std::ios::binary);
      image.writeBitmapDataToStream(ostream);
    }

    const auto fileWritten = std::chrono::steady_clock::now();

    std::cout << "Done" << std::endl;
    std::cout << "Image generation took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     imageComplete - start)
              << std::endl;
    std::cout << "File writing took: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     fileWritten - imageComplete)
              << std::endl;
    std::cout << "Total time:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     fileWritten - start)
              << std::endl;
  }

	return 0;
}
