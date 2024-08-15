#include "Array.h"

namespace glr {

VI_OBJECT_META_IMPL(Array, BufferObject);

Array::~Array() {
}

void Array::onBind(State& state) {
    glBindBuffer(GL_ARRAY_BUFFER, getId(state));
}

void Array::onUnbind(State& state) {
    GLint current_id = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_id);
    if (current_id == getId(state)) glBindTexture(GL_ARRAY_BUFFER, 0);
}

void Array::onRelease(State& state) {
    auto id = getId(state);
    glDeleteBuffers(1, &id);
}

BufferObject::Target Array::getTarget() const {
    return TARGET_ARRAY_BUFFER;
}

BufferObject::Usage Array::getUsage() const {
    return USAGE_STATIC_DRAW;
}

bool Array::isEmpty() const {
    return getSize() == 0;
}
} // namespace glr