#include "Flags.h"

#include <string_view>
#include <vector>

namespace voronoi {

Flags voronoi::parseFlags(int argc, const char** argv) {
  std::vector<std::string_view> args;
  args.reserve(argc);

  for (int i = 0; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  // Populate defaults
  Flags flags{
    .width=1024,
    .height=1024,
    .outputFile = "out.bmp",
    .numCells=100
  };

  return flags;
}

}  // namespace voronoi
