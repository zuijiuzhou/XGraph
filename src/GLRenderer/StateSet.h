#pragma once

#include "glrenderer_global.h"

#include "Object.h"
#include "StateAttribute.h"

namespace glr {
class StateAttribute;
class Shader;
class GLRENDERER_API StateSet : public Object {
    VI_OBJECT_META;

  public:
    StateSet();
    virtual ~StateSet();

  public:
    void setAttribute(StateAttribute* attr);

    StateAttribute* getAttribute(StateAttribute::Type type) const;

    size_t          getNbAttributes() const;
    StateAttribute* getAttributeAt(size_t i);

    Shader* getShader() const;

    void setShader(Shader* shader);

  private:
    VI_OBJECT_DATA;
};
} // namespace glr