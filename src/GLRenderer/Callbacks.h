#pragma once

#include "glrenderer_global.h"

#include "Object.h"

namespace glr {

class Renderer;

class GLRENDERER_API UpdateContext {
  public:
    virtual Renderer* getMasterRenderer() const = 0;
    virtual Renderer* getCurrentRenderer() const = 0;
};

class GLRENDERER_API UpdateCallback : public Object {
    VI_OBJECT_META;

  public:
    virtual void operator()(Object* obj, UpdateContext* ctx);
};
} // namespace glr