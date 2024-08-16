#pragma once

#include "glrenderer_global.h"

#include "SceneNode.h"

namespace glr {
class State;
class GLRENDERER_API Drawable : public SceneNode {
    VI_OBJECT_META;

  public:
    virtual void draw(State& ctx) = 0;
};
} // namespace glr