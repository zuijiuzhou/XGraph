#pragma once
#include "glrenderer_global.h"

#include <string>

#include <glad/glad.h>
#include <glm/fwd.hpp>

#include "StateAttribute.h"

namespace glr {
class GLRENDERER_API Uniform : public StateAttribute {
    VI_OBJECT_META;

  public:
    enum ValueType
    {
        UNDEFINED = 0,

        BOOL  = GL_BOOL,      // bool
        BVEC2 = GL_BOOL_VEC2, // bvec2
        BVEC3 = GL_BOOL_VEC3, // bvec3
        BVEC4 = GL_BOOL_VEC4, // bvec4

        INT   = GL_INT,      // int
        IVEC2 = GL_INT_VEC2, // ivec2
        IVEC3 = GL_INT_VEC3, // ivec3
        IVEC4 = GL_INT_VEC4, // ivec4

        FLOAT   = GL_FLOAT,      // float
        FVEC2   = GL_FLOAT_VEC2, // vec2
        FVEC3   = GL_FLOAT_VEC3, // vec3
        FVEC4   = GL_FLOAT_VEC4, // vec4
        FMAT3X3 = GL_FLOAT_MAT3, // mat3
        FMAT4X4 = GL_FLOAT_MAT4, // mat4

        DOUBLE  = GL_DOUBLE,      // double
        DVEC2   = GL_DOUBLE_VEC2, // dvec2
        DVEC3   = GL_DOUBLE_VEC3, // dvec3
        DVEC4   = GL_DOUBLE_VEC4, // dvec4
        DMAT3X3 = GL_DOUBLE_MAT3, // dmat3
        DMAT4X4 = GL_DOUBLE_MAT4, // dmat4
    };

  public:
    Uniform();
    Uniform(const std::string& name, bool val);
    Uniform(const std::string& name, const glm::bvec2& val);
    Uniform(const std::string& name, const glm::bvec3& val);
    Uniform(const std::string& name, const glm::bvec4& val);

    Uniform(const std::string& name, int val);
    Uniform(const std::string& name, const glm::ivec2& val);
    Uniform(const std::string& name, const glm::ivec3& val);
    Uniform(const std::string& name, const glm::ivec4& val);

    Uniform(const std::string& name, float val);
    Uniform(const std::string& name, const glm::fvec2& val);
    Uniform(const std::string& name, const glm::fvec3& val);
    Uniform(const std::string& name, const glm::fvec4& val);
    Uniform(const std::string& name, const glm::fmat3x3& val);
    Uniform(const std::string& name, const glm::fmat4x4& val);

    Uniform(const std::string& name, double val);
    Uniform(const std::string& name, const glm::dvec2& val);
    Uniform(const std::string& name, const glm::dvec3& val);
    Uniform(const std::string& name, const glm::dvec4& val);
    Uniform(const std::string& name, const glm::dmat3x3& val);
    Uniform(const std::string& name, const glm::dmat4x4& val);

    virtual ~Uniform();

  public:
    virtual Type getType() const override;

    ValueType getValueType() const;

    void setValue(bool val);
    void setValue(const glm::bvec2& val);
    void setValue(const glm::bvec3& val);
    void setValue(const glm::bvec4& val);

    void setValue(int val);
    void setValue(const glm::ivec2& val);
    void setValue(const glm::ivec3& val);
    void setValue(const glm::ivec4& val);

    void setValue(float val);
    void setValue(const glm::fvec2& val);
    void setValue(const glm::fvec3& val);
    void setValue(const glm::fvec4& val);
    void setValue(const glm::fmat3x3& val);
    void setValue(const glm::fmat4x4& val);

    void setValue(double val);
    void setValue(const glm::dvec2& val);
    void setValue(const glm::dvec3& val);
    void setValue(const glm::dvec4& val);
    void setValue(const glm::dmat3x3& val);
    void setValue(const glm::dmat4x4& val);

    virtual void apply(State& state) const override;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr