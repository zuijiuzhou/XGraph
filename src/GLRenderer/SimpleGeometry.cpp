#include "SimpleGeometry.h"

namespace glr {
VI_OBJECT_META_IMPL(SimpleGeometry, Geometry);

SimpleGeometry::SimpleGeometry() {
}
SimpleGeometry::~SimpleGeometry() {
}
void SimpleGeometry::setVertices(Vec3fArray* vertices) {
    addVertexAttribArray(0, vertices);
}
void SimpleGeometry::setNormals(Vec3fArray* normals) {
    addVertexAttribArray(0, normals);
}
void SimpleGeometry::setColors(Vec4fArray* colors) {
    addVertexAttribArray(0, colors);
}
} // namespace glr