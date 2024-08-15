#pragma once

#include "glrenderer_global.h"

#include "GLObject.h"

namespace glr {
class GLRENDERER_API Buffer : public GLObject {
    VI_OBJECT_META;

  public:
    Buffer();

  public:
    bool isDirty(State& state) const;
    void dirty();

    void bind(State& state);
    void unbind(State& state);
    void update(State& state);

  protected:
    virtual bool onUpdate(State& state) = 0;
    virtual void onBind(State& state)   = 0;
    virtual void onUnbind(State& state) = 0;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr