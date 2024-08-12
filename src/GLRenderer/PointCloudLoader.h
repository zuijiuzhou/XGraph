#pragma once
#include <string>

namespace glr {
class Model;
class PointCloudLoader {
  public:
    Model* load(const std::string& file);

    static bool isSupported(const std::string& file);
};
} // namespace glr