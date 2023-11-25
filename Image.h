#pragma once

#include <ostream>
#include <vector>

namespace voronoi {

struct Color {
  Color()
      : b(0.0),
        g(0.0),
        r(0.0) {}

  Color(float r, float g, float b) : b(b), g(g), r(r) {}

  Color(float brightness) : b(brightness), g(brightness), r(brightness) {}

  void clamp() {
    b = std::min(1.0f, std::max(0.0f, b));
    g = std::min(1.0f, std::max(0.0f, g));
    r = std::min(1.0f, std::max(0.0f, r));
  }

  float b;
  float g;
  float r;
};

class Image {
 public:
  Image(int width, int height)
      : width_(width),
        height_(height),
        data_(width * height, Color()) {}

  void setPixel(int x, int y, Color newColor) {
    data_[x + y * width_] = newColor;
  }

  void writeBitmapDataToStream(std::ostream& stream) const;

 private:
  int width_;
  int height_;
  std::vector<Color> data_;
};

} // namespace voronoi
