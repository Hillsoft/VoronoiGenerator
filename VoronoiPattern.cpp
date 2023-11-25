#include "VoronoiPattern.h"

namespace voronoi {

namespace {

template<typename TNum>
TNum sqr(TNum x) {
  return x * x;
}

struct Point {
  int x;
  int y;
};

}  // namespace

Image generateVoronoiPattern(int width, int height, int numCells, PatternType type) {
  Image result{width, height};

  std::vector<Point> cellCentres;
  cellCentres.reserve(numCells);

  for (int i = 0; i < numCells; i++) {
    cellCentres.emplace_back(rand() % width, rand() % height);
  }

  const int maxDist = sqr(width) + sqr(height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Point myCell{0, 0};
      int minDistance2 = maxDist;
      for (const auto& cell : cellCentres) {
        int curDist2 = sqr(x - cell.x) + sqr(y - cell.y);
        if (curDist2 < minDistance2) {
          myCell = cell;
          minDistance2 = curDist2;
        }
      }

      switch (type) {
        case PatternType::Distance:
          result.setPixel(x, y,
                          Color(std::sqrt(static_cast<float>(minDistance2)) /
                                static_cast<float>(width)));
          break;

        case PatternType::Centre:
          result.setPixel(
              x, y,
              Color(static_cast<float>(myCell.x) / static_cast<float>(width),
                    static_cast<float>(myCell.y) / static_cast<float>(height),
                    0.0f));
          break;
      }
    }
  }

  return result;
}

}  // namespace voronoi
