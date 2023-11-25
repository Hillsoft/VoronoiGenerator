#include <fstream>
#include <iostream>

#include "Flags.h"
#include "Image.h"

using namespace voronoi;

int main(int argc, const char** argv) {
  Flags flags = parseFlags(argc, argv);

  Image image(flags.width, flags.height);

  std::ofstream ostream(flags.outputFile, std::ios::out, std::ios::binary);
  image.writeBitmapDataToStream(ostream);

	return 0;
}
