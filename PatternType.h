#pragma once

#include <optional>
#include <string_view>

namespace voronoi {

enum class PatternType {
  Distance,
  Centre,
  Offset,
  OffsetRandomRotation,
  Border
};

void printPatternTypeHelp();
std::optional<PatternType> parsePatternType(std::string_view s);

}  // namespace voronoi
