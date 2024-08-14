#pragma once

#include "Object.h"

namespace glr {

class Renderer;

class UpdateContext {
  public:
    virtual Renderer* getCurrentRenderer() const = 0;
};

class UpdateCallback : public Object {
    VI_OBJECT_META;

  public:
    virtual void operator()(Object* obj, UpdateContext* ctx);
};
} // namespace glr