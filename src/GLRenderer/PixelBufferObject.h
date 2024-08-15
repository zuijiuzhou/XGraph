#pragma once

#include "glrenderer_global.h"

#include "BufferObject.h"

namespace glr {
class GLRENDERER_API PixelBufferObject : public BufferObject {
    VI_OBJECT_META;

  public:
  public:
    PixelBufferObject();

  private:
    VI_OBJECT_DATA;
};
} // namespace glr