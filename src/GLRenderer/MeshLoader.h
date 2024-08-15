#pragma once

#include "glrenderer_global.h"

#include <string>
#include <vector>

namespace glr {

class Model;
class GLRENDERER_API MeshLoader {
  public:
    Model* loadFile(const std::string& file);

    static bool isSupported(const std::string& file);
};
} // namespace glr