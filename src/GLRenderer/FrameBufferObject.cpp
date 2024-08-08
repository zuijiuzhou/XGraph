#include "FrameBufferObject.h"

#include <map>

#include <vine/core/Exception.h>
#include <vine/core/Ptr.h>

#include "RenderBuffer.h"
#include "State.h"
#include "Texture2D.h"

namespace glr {
VI_OBJECT_META_IMPL(FrameBufferObject, Buffer);

struct FrameBufferObject::Data {
    std::map<BufferComponent, vine::RefPtr<PixelData>> components;
};

FrameBufferObject::FrameBufferObject()
  : d(new Data()) {
}

FrameBufferObject::~FrameBufferObject() {
    delete d;
}

void FrameBufferObject::attachRenderBuffer(BufferComponent comp, RenderBuffer* buffer) {
    if (getNbInstances()) {
        throw vine::Exception(vine::Exception::INVALID_OPERATION);
    }
    auto iter = d->components.find(comp);
    if (iter == d->components.end())
        d->components.insert({ comp, buffer });
    else
        iter->second = buffer;
}

void FrameBufferObject::attachTexture(BufferComponent comp, Texture* tex) {
    if (getNbInstances()) {
        throw vine::Exception(vine::Exception::INVALID_OPERATION);
    }
    auto iter = d->components.find(comp);
    if (iter == d->components.end())
        d->components.insert({ comp, tex });
    else
        iter->second = tex;
}

GLuint FrameBufferObject::onCreate(State& state) {
    GLuint id;
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    for (auto&& kv : d->components) {
        auto& buffer = kv.second;
        buffer->bind(state);
        if (buffer->isKindOf<Texture2D>()) {
            glFramebufferTexture2D(GL_FRAMEBUFFER, kv.first, GL_TEXTURE_2D, buffer->getId(state), 0);
        }
        else if (buffer->isKindOf<RenderBuffer>()) {
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, kv.first, GL_RENDERBUFFER, buffer->getId(state));
        }
        buffer->unbind(state);
    }
    return id;
}

void FrameBufferObject::onRelease(State& state) {
    auto id = getId(state);
    glDeleteFramebuffers(1, &id);
}

bool FrameBufferObject::onUpdate(State& state) {
    return false;
}

void FrameBufferObject::onBind(State& state) {
    auto id = getId(state);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FrameBufferObject::onUnbind(State& state) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
} // namespace glr