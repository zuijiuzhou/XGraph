#include "Array.h"

#ifdef ArrayImpl

namespace glr {
VI_OBJECT_META_IMPL(ArrayImpl, Array);

GLuint ArrayImpl::onCreate(State& state) {
    GLuint id = 0;
    glGenBuffers(1, &id);
    glBindBuffer(getTarget(), id);
    glBufferData(getTarget(), size() * getSizeOfItem(), (void*)data(), getUsage());
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    return id;
}

bool ArrayImpl::onUpdate(State& state) {
    return true;
}

Array::Type ArrayImpl::getType() const {
    if constexpr (std::is_same<float, item_type>::value) {
        return Array::ARRAY_FLOAT;
    }

    else if constexpr (std::is_same<int8_t, item_type>::value) {
        return Array::ARRAY_INT8;
    }
    else if constexpr (std::is_same<int16_t, item_type>::value) {
        return Array::ARRAY_INT16;
    }
    else if constexpr (std::is_same<int32_t, item_type>::value) {
        return Array::ARRAY_INT32;
    }

    else if constexpr (std::is_same<uint8_t, item_type>::value) {
        return Array::ARRAY_UINT8;
    }
    else if constexpr (std::is_same<uint16_t, item_type>::value) {
        return Array::ARRAY_UINT16;
    }
    else if constexpr (std::is_same<uint32_t, item_type>::value) {
        return Array::ARRAY_UINT32;
    }

    else if constexpr (std::is_same<glm::vec2, item_type>::value) {
        return Array::ARRAY_VEC2F;
    }
    else if constexpr (std::is_same<glm::vec3, item_type>::value) {
        return Array::ARRAY_VEC3F;
    }
    else if constexpr (std::is_same<glm::vec4, item_type>::value) {
        return Array::ARRAY_VEC4F;
    }
    return Array::ARRAY_UNKNOW;
}

GLsizei ArrayImpl::getSize() const {
    return static_cast<GLuint>(size());
}

GLsizei ArrayImpl::getSizeOfItem() const {
    return static_cast<GLuint>(sizeof(item_type));
}

void* ArrayImpl::getData() {
    return (void*)data();
}

void* ArrayImpl::getAt(GLsizei index) {
    return &at(index);
}
} // namespace glr
#endif
