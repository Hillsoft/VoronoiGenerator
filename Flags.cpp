#include "Flags.h"

#include <charconv>
#include <iostream>
#include <string_view>
#include <vector>

namespace voronoi {

namespace {

void printHelp() {
  std::cout << "Usage:" << std::endl;
  std::cout << "  VoronoiGenerator [--help] <outputFile> <width> <height> "
               "[--numCells=10] [--distanceScaleFactor=1] [--type=distance] [--tile]"
            << std::endl;
}

}  // namespace

std::optional<Flags> voronoi::parseFlags(int argc, const char** argv) {
  std::vector<std::string_view> args;
  args.reserve(argc);

  for (int i = 1; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  // Populate defaults
  Flags flags{.width = 1024,
              .height = 1024,
              .outputFile = "out.bmp",
              .numCells = 10,
              .distanceScaleFactor = 1,
              .patternType = PatternType::Distance,
              .tile = false};

  int currentPosition = 0;

  for (const auto& a : args) {
    if (a.substr(0, 2) == "--") {
      // Handle named argument
      std::string_view argContent = a.substr(2);
      size_t eqSplit = argContent.find('=');

      std::string_view argName = eqSplit == std::string_view::npos
                                     ? argContent
                                     : argContent.substr(0, eqSplit);
      std::string_view argValue = eqSplit == std::string_view::npos
                                      ? ""
                                      : argContent.substr(eqSplit + 1);

      if (argName == "help") {
        printHelp();
        return std::nullopt;
      } else if (argName == "numCells") {
        auto convertResult = std::from_chars(
            argValue.data(), argValue.data() + argValue.size(), flags.numCells);
        if (convertResult.ptr != argValue.data() + argValue.size()) {
          std::cout << "Not a valid value for '" << argName
                    << "' of type integer: " << argValue << std::endl;
          return std::nullopt;
        }
      } else if (argName == "type") {
        std::optional<PatternType> type = parsePatternType(argValue);
        if (type.has_value()) {
          flags.patternType = *type;
        } else {
          std::cout << "Not a valid value for '" << argName << "'" << std::endl;
          printPatternTypeHelp();
          return std::nullopt;
        }
      } else if (argName == "tile") {
        flags.tile = true;
      } else if (argName == "distanceScaleFactor") {
        auto convertResult = std::from_chars(
            argValue.data(), argValue.data() + argValue.size(), flags.distanceScaleFactor);
        if (convertResult.ptr != argValue.data() + argValue.size()) {
          std::cout << "Not a valid value for '" << argName
                    << "' of type integer: " << argValue << std::endl;
          return std::nullopt;
        }
      }
      else {
        std::cout << "Not a valid argument: " << argName << std::endl;
        return std::nullopt;
      }
    } else {
      // Handle positional argument
      switch (currentPosition) {
        case 0:
          // output file
          if (!a.ends_with(".bmp")) {
            std::cout << "Only '.bmp' output is supported" << std::endl;
            return std::nullopt;
          }

          flags.outputFile = std::string{a};
          break;

        case 1:
          // width
          {
            auto convertResult =
                std::from_chars(a.data(), a.data() + a.size(), flags.width);
            if (convertResult.ptr != a.data() + a.size()) {
              std::cout << "Not a valid value for 'width' of type integer: "
                        << a << std::endl;
              return std::nullopt;
            }
          }
          break;

        case 2:
          // height
          {
            auto convertResult =
                std::from_chars(a.data(), a.data() + a.size(), flags.height);
            if (convertResult.ptr != a.data() + a.size()) {
              std::cout << "Not a valid value for 'height' of type integer: "
                        << a << std::endl;
              return std::nullopt;
            }
          }
          break;

        default:
          std::cout << "Too many arguments" << std::endl;
          return std::nullopt;
      }

      currentPosition++;
    }
  }

  if (currentPosition != 3) {
    std::cout << "Not all mandatory arguments provided" << std::endl;
    printHelp();
    return std::nullopt;
  }

  return flags;
}

}  // namespace voronoi
