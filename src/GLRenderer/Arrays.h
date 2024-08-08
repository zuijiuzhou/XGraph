#pragma once

#include "Array.h"
#include "stdrenderer_global.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace glr {

#define DEF_ARR(N, T)                                                                                                  \
    class N##Array : public Array, public std::vector<T> {                                                             \
        VI_OBJECT_META                                                                                                 \
      public:                                                                                                          \
        using item_type = T;                                                                                           \
                                                                                                                       \
      public:                                                                                                          \
        virtual Type    getType() const;                                                                               \
        virtual GLsizei getSize() const override;                                                                      \
        virtual GLsizei getSizeOfItem() const override;                                                                \
        virtual void*   getData() override;                                                                            \
        virtual void*   getAt(GLsizei index) override;                                                                 \
                                                                                                                       \
      protected:                                                                                                       \
        virtual GLuint onCreate(State& state) override;                                                                \
        virtual bool   onUpdate(State& state) override;                                                                \
    };

DEF_ARR(Int8, int8_t);
DEF_ARR(Int16, int16_t);
DEF_ARR(Int32, int32_t);

DEF_ARR(UInt8, uint8_t);
DEF_ARR(UInt16, uint16_t);
DEF_ARR(UInt32, uint32_t);

DEF_ARR(Float, float);
DEF_ARR(Vec2f, glm::vec2);
DEF_ARR(Vec3f, glm::vec3);
DEF_ARR(Vec4f, glm::vec4);
} // namespace glr
