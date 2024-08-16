#pragma once

#include "glrenderer_global.h"

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLObject.h"

namespace glr {
class GLRENDERER_API Shader : public GLObject {
    VI_OBJECT_META;

  public:
    Shader(const std::string& vs_code, const std::string& gs_code, const std::string& fs_code);
    virtual ~Shader();

  public:
    void use(State& state);
    void unuse(State& state);

    std::string getName() const;
    void        setName(const std::string& name);

    template <typename T> void set(State& state, const std::string& name, const T& val) {
        auto id  = getId(state);
        auto loc = glGetUniformLocation(id, name.data());
        if (loc >= 0) {
            set<T>(state, loc, val);
        }
    }

    template <typename T> void set(State& state, GLuint loc, const T& val) {
        if constexpr (std::is_same<T, bool>::value) {
            glUniform1i(loc, (int)val);
        }
        else if constexpr (std::is_same<T, glm::bvec2>::value) {
            glUniform2i(loc, val.x, val.y);
        }
        else if constexpr (std::is_same<T, glm::bvec3>::value) {
            glUniform3i(loc, val.x, val.y, val.z);
        }
        else if constexpr (std::is_same<T, glm::bvec4>::value) {
            glUniform4i(loc, val.r, val.g, val.b, val.a);
        }

        else if constexpr (std::is_same<T, int>::value) {
            glUniform1i(loc, val);
        }
        else if constexpr (std::is_same<T, glm::ivec2>::value) {
            glUniform2i(loc, val.x, val.y);
        }
        else if constexpr (std::is_same<T, glm::ivec3>::value) {
            glUniform3i(loc, val.x, val.y, val.z);
        }
        else if constexpr (std::is_same<T, glm::ivec4>::value) {
            glUniform4i(loc, val.r, val.g, val.b, val.a);
        }

        else if constexpr (std::is_same<T, float>::value) {
            glUniform1f(loc, val);
        }
        else if constexpr (std::is_same<T, glm::fvec2>::value) {
            glUniform2f(loc, val.x, val.y);
        }
        else if constexpr (std::is_same<T, glm::fvec3>::value) {
            glUniform3f(loc, val.x, val.y, val.z);
        }
        else if constexpr (std::is_same<T, glm::fvec4>::value) {
            glUniform4f(loc, val.r, val.g, val.b, val.a);
        }
        else if constexpr (std::is_same<T, glm::fmat3x3>::value) {
            glUniformMatrix3fv(loc, 1, GL_FALSE, glm::value_ptr(val));
        }
        else if constexpr (std::is_same<T, glm::fmat4x4>::value) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
        }

        else if constexpr (std::is_same<T, double>::value) {
            glUniform1d(loc, val);
        }
        else if constexpr (std::is_same<T, glm::dvec2>::value) {
            glUniform2d(loc, val.x, val.y);
        }
        else if constexpr (std::is_same<T, glm::dvec3>::value) {
            glUniform3d(loc, val.x, val.y, val.z);
        }
        else if constexpr (std::is_same<T, glm::dvec4>::value) {
            glUniform4d(loc, val.r, val.g, val.b, val.a);
        }    
        else if constexpr (std::is_same<T, glm::dmat3x3>::value) {
            glUniformMatrix3dv(loc, 1, GL_FALSE, glm::value_ptr(val));
        }
        else if constexpr (std::is_same<T, glm::dmat4x4>::value) {
            glUniformMatrix4dv(loc, 1, GL_FALSE, glm::value_ptr(val));
        }
        else {
            static_assert("type not supported");
        }
    }

  public:
    static Shader* create(const std::string& vs_path, const std::string& gs_path, const std::string& fs_path);

  protected:
    GLuint onCreate(State& state);
    void   onRelease(State& state) override;

  private:
    struct Data;
    Data* const d;
};
} // namespace glr