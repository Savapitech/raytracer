#include "Ppm.hpp"
#include "logger.hpp"
#include <fstream>
#include <stdexcept>



namespace object {}
namespace shape {}
namespace space {}
namespace material {}
namespace light {}
namespace scene {}
namespace render {}
namespace graphical {}

namespace graphical {
    using namespace scene;
    using namespace object;
    using namespace space;
    using namespace shape;
    using namespace material;

Ppm::Ppm(const std::string &filename) : _filename(filename), _saved(false) {
  Log::Logger::info("PPM output: " + filename);
}

void Ppm::save(const uint8_t *data, uint32_t width, uint32_t height) {
  std::ofstream file(_filename, std::ios::binary);
  if (!file)
    throw std::runtime_error("Cannot open output file: " + _filename);

  file << "P6\n" << width << " " << height << "\n255\n";
  for (uint32_t i = 0; i < width * height; i++)
    file.write(reinterpret_cast<const char *>(data + i * 4), 3);

  Log::Logger::info("PPM saved: " + _filename);
  _saved = true;
}

} // namespace graphical
