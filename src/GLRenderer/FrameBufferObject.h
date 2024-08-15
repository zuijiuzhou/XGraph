#pragma once

#include "glrenderer_global.h"

#include "Buffer.h"

namespace glr {
class RenderBuffer;
class Texture;

class GLRENDERER_API FrameBufferObject : public Buffer {
    VI_OBJECT_META;

  public:
    enum BufferComponent
    {
        DEPTH_ATTACHMENT         = GL_DEPTH_ATTACHMENT,
        STENCIL_ATTACHMENT       = GL_STENCIL_ATTACHMENT,
        DEPTH_STENCIL_ATTACHMENT = GL_DEPTH_STENCIL_ATTACHMENT,
        COLOR_ATTACHMENT0        = GL_COLOR_ATTACHMENT0,
        COLOR_ATTACHMENT1        = GL_COLOR_ATTACHMENT1,
        COLOR_ATTACHMENT2        = GL_COLOR_ATTACHMENT2,
        COLOR_ATTACHMENT3        = GL_COLOR_ATTACHMENT3,
        COLOR_ATTACHMENT4        = GL_COLOR_ATTACHMENT4,
        COLOR_ATTACHMENT5        = GL_COLOR_ATTACHMENT5,
        COLOR_ATTACHMENT6        = GL_COLOR_ATTACHMENT6,
        COLOR_ATTACHMENT7        = GL_COLOR_ATTACHMENT7,
    };

  public:
    FrameBufferObject();
    virtual ~FrameBufferObject();

  public:
    void attachRenderBuffer(BufferComponent comp, RenderBuffer* buffer);
    void attachTexture(BufferComponent comp, Texture* tex);

  protected:
    virtual GLuint onCreate(State& state) override;
    virtual void   onRelease(State& state) override;
    virtual bool   onUpdate(State& state) override;
    virtual void   onBind(State& state) override;
    virtual void   onUnbind(State& state) override;



  private:
    VI_OBJECT_DATA;
};
} // namespace glr