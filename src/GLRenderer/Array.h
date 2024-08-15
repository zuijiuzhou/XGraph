#pragma once

#include "glrenderer_global.h"

#include "BufferObject.h"

namespace glr {
class GLRENDERER_API Array : public BufferObject {
    VI_OBJECT_META;

  public:
    enum Type
    {
        ARRAY_UNKNOW = 0,

        ARRAY_INT8,
        ARRAY_INT16,
        ARRAY_INT32,

        ARRAY_UINT8,
        ARRAY_UINT16,
        ARRAY_UINT32,

        ARRAY_FLOAT,

        ARRAY_VEC2F,
        ARRAY_VEC3F,
        ARRAY_VEC4F
    };

  public:
    virtual ~Array();

  public:
    virtual Type    getType() const       = 0;
    virtual GLsizei getSize() const       = 0;
    virtual GLsizei getSizeOfItem() const = 0;
    virtual void*   getData()             = 0;
    virtual void*   getAt(GLsizei index)  = 0;

    virtual Target getTarget() const override;
    virtual Usage  getUsage() const override;

    bool isEmpty() const;

  protected:
    virtual void onBind(State& state) override;
    virtual void onUnbind(State& state) override;
    virtual void onRelease(State& state) override;
};
} // namespace glr