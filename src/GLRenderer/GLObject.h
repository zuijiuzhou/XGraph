#pragma once
#include "Object.h"
#include <glad/glad.h>

namespace glr {
class State;
class GLObject : public Object {
    friend class State;

    VI_OBJECT_META;
    VI_DISABLE_COPY_MOVE(GLObject);

  public:
    GLObject();
    virtual ~GLObject();

  public:
    GLuint getId(State& state) const;
    bool   isCreated(State& state) const;
    GLuint getNbInstances() const;

  protected:
    void           create(State& state);
    virtual GLuint onCreate(State& state) = 0;
    void           release(State& state);
    virtual void   onRelease(State& state) = 0;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr