#pragma once
#include <string>

namespace glr {
class Model;
class PointCloudLoader {

  public:
    PointCloudLoader(const std::string& path);

  public:
    Model* getData() const;

  private:
    void load(const std::string& path);

  private:
    Model* data_ = nullptr;
};
} // namespace glr