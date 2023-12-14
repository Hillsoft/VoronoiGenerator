#include "VoronoiPattern.h"

#include <array>
#include <compare>
#include <ranges>

#include "Assert.h"

namespace voronoi {

namespace {

template<typename TNum>
TNum sqr(TNum x) {
  return x * x;
}

template<typename TNum>
TNum wraparound(TNum x, TNum interval) {
  TNum result = x % interval;
  if (result < 0) {
    result += interval;
  }
  return result;
}

template<typename TNum>
TNum wraparoundDist(TNum a, TNum b, TNum interval) {
  return std::min(wraparound(a - b, interval), wraparound(b - a, interval));
}

struct Point {
  int x;
  int y;
};

struct DistanceInfo {
 public:
  constexpr DistanceInfo(int xDist2, int yDist2)
      : xDist2_(xDist2), yDist2_(yDist2), dist2_(xDist2 + yDist2) {}

  constexpr int getXDist2() const { return xDist2_; }
  constexpr int getYDist2() const { return yDist2_; }
  constexpr int getDist2() const { return dist2_; }

  constexpr auto operator<=>(const DistanceInfo& other) const {
    return dist2_ <=> other.dist2_;
  }

 private:
  int xDist2_;
  int yDist2_;
  int dist2_;
};

// We divide the image into a grid and each grid cell gets a single cell centre
std::vector<Point> generateCellCentres(int width, int height, int numCells) {
  std::vector<Point> result;
  result.reserve(sqr(numCells));

  for (int i = 0; i < sqr(numCells); i++) {
    int cellX = i % numCells;
    int cellY = i / numCells;

    int cellMinX = (cellX * width) / numCells;
    int cellMaxX = ((cellX + 1) * width) / numCells;

    int cellMinY = (cellY * width) / numCells;
    int cellMaxY = ((cellY + 1) * width) / numCells;

    result.emplace_back(cellMinX + rand() % (cellMaxX - cellMinX),
                        cellMinY + rand() % (cellMaxY - cellMinY));
  }

  return result;
}

// Returns the nearest cell centre and the distance to that centre
std::pair<Point, DistanceInfo> getNearsetCellCentre(int width, int height, int numCells,
                           std::span<const Point> cellCentres, Point point, bool tile) {
  assert("numCells must match amount of cellCentres",
         cellCentres.size() == sqr(numCells));

  DistanceInfo maxDist{sqr(width), sqr(height)};

  int pointGridX = numCells * point.x / width;
  int pointGridY = numCells * point.y / height;

  std::pair<Point, DistanceInfo> best{Point{0, 0}, maxDist};

  // We know the nearest cell centre must be in one of the nine nearby grid cells
  for (int gridX = pointGridX - 1; gridX <= pointGridX + 1; gridX++) {
    for (int gridY = pointGridY - 1; gridY <= pointGridY + 1; gridY++) {
      int curGridIndex = wraparound(gridX, numCells) + numCells * wraparound(gridY, numCells);
      Point curCellCentre = cellCentres[curGridIndex];

      int xDist2 = tile ? sqr(wraparoundDist(curCellCentre.x, point.x, width))
                        : sqr(point.x - curCellCentre.x);
      int yDist2 = tile ? sqr(wraparoundDist(curCellCentre.y, point.y, height))
                        : sqr(point.y - curCellCentre.y);

      DistanceInfo curDist2{xDist2, yDist2};

      if (curDist2 < best.second) {
        best = {curCellCentre, curDist2};
      }
    }
  }

  return best;
}

}  // namespace

Image generateVoronoiPattern(int width, int height, int numCells, PatternType type, bool tile) {
  Image result{width, height};

  std::vector<Point> cellCentres = generateCellCentres(width, height, numCells);

  const int maxDist = sqr(width) + sqr(height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      auto [myCell, minDistance] = getNearsetCellCentre(
          width, height, numCells, cellCentres, {x, y}, tile);

      switch (type) {
        case PatternType::Distance:
          result.setPixel(x, y,
                          Color(std::sqrt(static_cast<float>(minDistance.getDist2())) /
                                static_cast<float>(width)));
          break;

        case PatternType::Centre:
          result.setPixel(
              x, y,
              Color(static_cast<float>(myCell.x) / static_cast<float>(width),
                    static_cast<float>(myCell.y) / static_cast<float>(height),
                    0.0f));
          break;

        case PatternType::Offset:
          result.setPixel(
              x, y,
              Color(std::sqrt(static_cast<float>(minDistance.getXDist2())) / static_cast<float>(width),
                    std::sqrt(static_cast<float>(minDistance.getYDist2())) / static_cast<float>(height),
                    0.0f));
          break;
      }
    }
  }

  return result;
}

}  // namespace voronoi
