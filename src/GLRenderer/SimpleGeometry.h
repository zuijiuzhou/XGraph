#pragma once

#include "glrenderer_global.h"

#include "Geometry.h"

namespace glr {
class GLRENDERER_API SimpleGeometry : public Geometry {
    VI_OBJECT_META;

  public:
    SimpleGeometry();
    virtual ~SimpleGeometry();

  public:
    void setVertices(Vec3fArray* vertices);
    void setNormals(Vec3fArray* normals);
    void setColors(Vec4fArray* colors);
};
} // namespace glr