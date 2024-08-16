#pragma once

#include "glrenderer_global.h"

#include <glm/vec3.hpp>

namespace glr {
class GLRENDERER_API BoundingBox {
  public:
    BoundingBox();
    BoundingBox(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

  public:
    glm::vec3 getCenter() const;
    double    getRadius() const;
    double    getXLength() const;
    double    getYLength() const;
    double    getZLength() const;

    void combine(const BoundingBox& box);
    void expandBy(const glm::vec3& pt);

    void set(double xmin, double ymin, double zmin, double xmax, double ymax, double zmax);

  private:
    glm::vec3 min_;
    glm::vec3 max_;
};
} // namespace glr