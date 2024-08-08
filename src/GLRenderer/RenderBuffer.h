#pragma once

#include "PixelData.h"

namespace glr {

class FrameBufferObject;

class RenderBuffer : public PixelData {

    VI_OBJECT_META;

    friend class FrameBufferObject;

  public:
    RenderBuffer();

    FrameBufferObject* getFrameBuffer() const;

  protected:
    virtual bool onUpdate(State& state) override;

    virtual void onBind(State& state) override;

    virtual void onUnbind(State& state) override;

    virtual GLuint onCreate(State& state) override;

    virtual void onRelease(State& state) override;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr