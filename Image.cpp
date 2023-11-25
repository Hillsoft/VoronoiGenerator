#include "Image.h"

#include <bit>

namespace voronoi {

namespace {

#pragma pack(push, 1)
struct BitmapHeader {
  char id1 = 'B';
  char id2 = 'M';
  int fileSize = 0;  // Set
  short reserved1 = 0;
  short reserved2 = 0;
  int startPos = 0x36;
  int subHeaderSize = 40;
  int width = 0;   // Set
  int height = 0;  // Set
  short colorplanes = 1;
  short bitsPerPixel = 24;  // Set
  int compression = 0;
  int dataSize = 0;  // Set
  int pixelPerMeterX = 0;
  int pixelPerMeterY = 0;
  int paletteSize = 0;
  int importantColors = 0;
};
#pragma pack(pop)

void writeBitmapHeader(std::ostream& stream, const BitmapHeader& header) {
  static_assert(
      std::endian::native == std::endian::little,
      "Currently only little endian bitmap header writing is supported");

  stream.write(reinterpret_cast<const char*>(&header),
               sizeof(header));
}

} // namespace


void Image::writeBitmapDataToStream(std::ostream& stream) const {
  int rowSize = (3 * width_ + 3) & ~0b11;
  int padSize = rowSize - (3 * width_);
  char padding[3] = {0, 0, 0};

  BitmapHeader header;
  header.bitsPerPixel = sizeof(char) * 3 * 8;
  header.dataSize = (header.bitsPerPixel / 8) * rowSize * height_;
  header.fileSize = header.dataSize + sizeof(header);
  header.width = width_;
  header.height = height_;

  writeBitmapHeader(stream, header);

  for (int y = 0; y < height_; y++) {
    for (int x = 0; x < width_; x++) {
      Color curCol = data_[y * width_ + x];
      curCol.clamp();

      unsigned char curVals[3] = {
          static_cast<unsigned char>(curCol.b * 255.0f),
          static_cast<unsigned char>(curCol.g * 255.0f),
          static_cast<unsigned char>(curCol.r * 255.0f)};
      stream.write(reinterpret_cast<const char*>(curVals), 3);
    }
    stream.write(padding, padSize);
  }
}

} // namespace voronoi