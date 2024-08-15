#pragma once

#include "glrenderer_global.h"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

#include "BoundingBox.h"
#include "Object.h"

namespace glr {
class Shader;
class StateSet;
class State;
class GLRENDERER_API Drawable : public Object {
    VI_OBJECT_META;

  public:
    virtual void draw(State& ctx) = 0;

    virtual BoundingBox getBoundingBox() const = 0;
};
} // namespace glr