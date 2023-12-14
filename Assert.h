#pragma once

#ifdef NDEBUG

inline constexpr void assert(std::string_view /* message */, bool condition) {
#ifdef _MSC_VER
  __assume(condition);
#endif
}

#else

#include <iostream>
  inline constexpr void assert(std::string_view message, bool condition) {
    if (!condition) {
      std::cout << "Assert failed: " << message << std::endl;
      std::abort();
    }
  }

#endif
