#include "PatternType.h"

#include <iostream>
#include <map>

namespace voronoi {

const std::map<std::string_view, PatternType> patternTypeNames{
    {"distance", PatternType::Distance},
    {"centre", PatternType::Centre},
    {"offset", PatternType::Offset},
    {"offsetRandomRotation", PatternType::OffsetRandomRotation},
    {"border", PatternType::Border}};

void printPatternTypeHelp() {
  std::cout << "Available pattern types are:" << std::endl;

  for (const auto& p : patternTypeNames) {
    std::cout << "  " << p.first << std::endl;
  }
}

std::optional<PatternType> parsePatternType(std::string_view s) {
  auto it = patternTypeNames.find(s);
  if (it != patternTypeNames.end()) {
    return it->second;
  }

  return std::nullopt;
}

}  // namespace voronoi
