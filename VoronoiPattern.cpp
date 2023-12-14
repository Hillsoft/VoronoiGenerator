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


bool minAbs(int a, int b) { return (std::abs(a) < std::abs(b)); }

struct Point {
  int x;
  int y;
};

struct CellCentre {
  Point location;
  int randomValue;
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
std::vector<CellCentre> generateCellCentres(int width, int height, int numCells) {
  std::vector<CellCentre> result;
  result.reserve(sqr(numCells));

  for (int i = 0; i < sqr(numCells); i++) {
    int cellX = i % numCells;
    int cellY = i / numCells;

    int cellMinX = (cellX * width) / numCells;
    int cellMaxX = ((cellX + 1) * width) / numCells;

    int cellMinY = (cellY * width) / numCells;
    int cellMaxY = ((cellY + 1) * width) / numCells;

    result.emplace_back(Point{cellMinX + rand() % (cellMaxX - cellMinX),
                              cellMinY + rand() % (cellMaxY - cellMinY)},
                        rand());
  }

  return result;
}

// Returns the nearest two cell centres and the distance to that centre
std::pair<std::pair<CellCentre, DistanceInfo>,
          std::pair<CellCentre, DistanceInfo>>
getNearsetCellCentre(int width, int height, int numCells,
                     std::span<const CellCentre> cellCentres, Point point,
                     bool tile) {
  assert("numCells must match amount of cellCentres",
         cellCentres.size() == sqr(numCells));

  DistanceInfo maxDist{sqr(width), sqr(height)};

  int pointGridX = numCells * point.x / width;
  int pointGridY = numCells * point.y / height;

  std::pair<std::pair<CellCentre, DistanceInfo>,
            std::pair<CellCentre, DistanceInfo>>
      result{{CellCentre{Point{0, 0}, 0}, maxDist},
             {CellCentre{Point{0, 0}, 0}, maxDist}};

  // We know the nearest cell centre must be in one of the nine nearby grid cells
  for (int gridX = pointGridX - 1; gridX <= pointGridX + 1; gridX++) {
    for (int gridY = pointGridY - 1; gridY <= pointGridY + 1; gridY++) {
      int curGridIndex = wraparound(gridX, numCells) + numCells * wraparound(gridY, numCells);
      CellCentre curCellCentre = cellCentres[curGridIndex];

      int xDist2 =
          tile ? sqr(wraparoundDist(curCellCentre.location.x, point.x, width))
               : sqr(point.x - curCellCentre.location.x);
      int yDist2 =
          tile ? sqr(wraparoundDist(curCellCentre.location.y, point.y, height))
               : sqr(point.y - curCellCentre.location.y);

      DistanceInfo curDist2{xDist2, yDist2};

      if (curDist2 < result.first.second) {
        result.second = result.first;
        result.first = {curCellCentre, curDist2};
      } else if (curDist2 < result.second.second) {
        result.second = {curCellCentre, curDist2};
      }
    }
  }

  return result;
}

}  // namespace

Image generateVoronoiPattern(int width, int height, int numCells, PatternType type, bool tile, int distanceScaleFactor) {
  Image result{width, height};

  std::vector<CellCentre> cellCentres = generateCellCentres(width, height, numCells);

  const int maxDist = sqr(width) + sqr(height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      std::pair<std::pair<CellCentre, DistanceInfo>,
                std::pair<CellCentre, DistanceInfo>> closestCellResult =
          getNearsetCellCentre(
          width, height, numCells, cellCentres, {x, y}, tile);

      CellCentre& myCell = closestCellResult.first.first;
      DistanceInfo& minDistance = closestCellResult.first.second;

      CellCentre& secondCell = closestCellResult.second.first;
      DistanceInfo& secondDistance = closestCellResult.second.second;

      switch (type) {
        case PatternType::Distance:
          result.setPixel(
              x, y,
              Color(static_cast<float>(distanceScaleFactor) *
                    std::sqrt(static_cast<float>(minDistance.getDist2())) /
                    static_cast<float>(width)));
          break;

        case PatternType::Centre:
          result.setPixel(
              x, y,
              Color(static_cast<float>(myCell.location.x) / static_cast<float>(width),
                    static_cast<float>(myCell.location.y) / static_cast<float>(height),
                    0.0f));
          break;

        case PatternType::Border:
          result.setPixel(
              x, y,
              Color(abs(sqrt(static_cast<float>(minDistance.getDist2())) -
                        sqrt(static_cast<float>(secondDistance.getDist2()))) /
                    2.0f));
          break;

        case PatternType::Offset: {
          int xDist = std::ranges::min(
              std::array<int, 3>{x - myCell.location.x,
                                 x + width - myCell.location.x,
                                 x - width - myCell.location.x},
              minAbs);
          int yDist = std::ranges::min(
              std::array<int, 3>{y - myCell.location.y,
                                 y + height - myCell.location.y,
                                 y - height - myCell.location.y},
              minAbs);

          result.setPixel(x, y,
                          Color(0.5f + static_cast<float>(distanceScaleFactor) *
                                           static_cast<float>(xDist) /
                                           static_cast<float>(width),
                                0.5f + static_cast<float>(distanceScaleFactor) *
                                           static_cast<float>(yDist) /
                                           static_cast<float>(height),
                                0.0f));
        }
          break;

        case PatternType::OffsetRandomRotation: {
          long double xDist = std::ranges::min(
              std::array<int, 3>{x - myCell.location.x,
                                 x + width - myCell.location.x,
                                 x - width - myCell.location.x},
              minAbs);
          long double yDist = std::ranges::min(
              std::array<int, 3>{y - myCell.location.y,
                                 y + height - myCell.location.y,
                                 y - height - myCell.location.y},
              minAbs);

          long double angle = static_cast<long double>(myCell.randomValue);
          long double xDistRot = cos(angle) * xDist + sin(angle) * yDist;
          long double yDistRot = sin(angle) * xDist - cos(angle) * yDist;

          result.setPixel(x, y,
                          Color(0.5f + static_cast<float>(distanceScaleFactor) *
                                           static_cast<float>(xDistRot) /
                                           static_cast<float>(width),
                                0.5f + static_cast<float>(distanceScaleFactor) *
                                           static_cast<float>(yDistRot) /
                                           static_cast<float>(height),
                                0.0f));
        }
          break;
      }
    }
  }

  return result;
}

}  // namespace voronoi
