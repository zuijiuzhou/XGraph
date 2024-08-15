#pragma once

#include "glrenderer_global.h"

#include "Buffer.h"

namespace glr {
class GLRENDERER_API BufferObject : public Buffer {
    VI_OBJECT_META;

  public:
    enum Target
    {
        TARGET_ARRAY_BUFFER          = GL_ARRAY_BUFFER,
        TARGET_ELEMENT_BUFFER        = GL_ELEMENT_ARRAY_BUFFER,
        TARGET_PIXEL_PACK_BUFFER     = GL_PIXEL_PACK_BUFFER,
        TARGET_PIXEL_UNPACK_BUFFER   = GL_PIXEL_UNPACK_BUFFER,
        TARGET_UNIFORM_BUFFER        = GL_UNIFORM_BUFFER,
        TARGET_SHADER_STORAGE_BUFFER = GL_SHADER_STORAGE_BUFFER,
        TARGET_DRAW_INDIRECT_BUFFER  = GL_DRAW_INDIRECT_BUFFER
    };

    enum Usage
    {
        USAGE_STREAM_DRAW  = GL_STREAM_DRAW,
        USAGE_STREAM_READ  = GL_STREAM_READ,
        USAGE_STREAM_COPY  = GL_STREAM_COPY,
        USAGE_STATIC_DRAW  = GL_STATIC_DRAW,
        USAGE_STATIC_READ  = GL_STATIC_READ,
        USAGE_STATIC_COPY  = GL_STATIC_COPY,
        USAGE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        USAGE_DYNAMIC_READ = GL_DYNAMIC_READ,
        USAGE_DYNAMIC_COPY = GL_DYNAMIC_COPY,
    };

  public:
    BufferObject();

  public:
    virtual Target getTarget() const = 0;
    virtual Usage  getUsage() const  = 0;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr