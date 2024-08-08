#pragma once
#include <string>
#include <vector>

namespace glr {

class Model;
class MeshLoader {
  public:
    Model* loadFile(const std::string& file);

    static bool isSupported(const std::string& file);
};
} // namespace glr