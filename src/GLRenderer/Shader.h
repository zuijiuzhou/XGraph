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
        else if constexpr (std::is_same<T, int>::value) {
            glUniform1i(loc, val);
        }
        else if constexpr (std::is_same<T, float>::value) {
            glUniform1f(loc, val);
        }
        else if constexpr (std::is_same<T, glm::vec3>::value) {
            glUniform3f(loc, val.x, val.y, val.z);
        }
        else if constexpr (std::is_same<T, glm::vec4>::value) {
            glUniform4f(loc, val.r, val.g, val.b, val.a);
        }
        else if constexpr (std::is_same<T, glm::mat4>::value) {
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(val));
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