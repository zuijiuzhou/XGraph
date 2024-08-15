#pragma once

#include "glrenderer_global.h"

#include <string>

namespace glr {
class Model;
class GLRENDERER_API PointCloudLoader {
  public:
    Model* load(const std::string& file);

    static bool isSupported(const std::string& file);
};
} // namespace glr