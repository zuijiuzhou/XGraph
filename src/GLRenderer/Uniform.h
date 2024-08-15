#pragma once
#include "glrenderer_global.h"

#include <string>

#include <glm/mat4x4.hpp>

#include "StateAttribute.h"

namespace glr {
class GLRENDERER_API Uniform : public StateAttribute {
    VI_OBJECT_META;

  public:
    enum ValueType
    {
        UNDEFINED = 0,
        BOOL,
        INT,
        MAT4X4
    };

  public:
    Uniform();
    Uniform(const std::string& name, int val);
    Uniform(const std::string& name, bool val);
    Uniform(const std::string& name, const glm::mat4x4& mat);

    virtual ~Uniform();

  public:
    virtual void apply(State& state) const override;

    virtual Type getType() const override;

    void setMat4x4(const glm::mat4x4& mat);

  private:
    VI_OBJECT_DATA;
};
} // namespace glr