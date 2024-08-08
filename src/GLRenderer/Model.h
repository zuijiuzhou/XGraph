#pragma once
#include "BoundingBox.h"
#include "Object.h"
#include <glm/mat4x4.hpp>
#include <vector>

namespace glr {
class Drawable;
class StateSet;
class State;
class ModelCallback;

class Model : public Object {
    VI_OBJECT_META;

  public:
    Model();
    virtual ~Model();

  public:
    glm::mat4 getMatrix() const;

    void setMatrix(const glm::mat4& mat);

    StateSet* getOrCreateStateSet();

    StateSet* getStateSet();

    void addDrawable(Drawable* drawable);

    void removeDrawable(Drawable* drawable);

    int getNbDrawables() const;

    Drawable* getDrawableAt(int index) const;

    BoundingBox getBoundingBox() const;

    void addCallback(ModelCallback* callback);

    void removeCallback(ModelCallback* callback);

    void update(State& ctx);

  private:
    VI_OBJECT_DATA;
};

class ModelCallback : public Object {
  public:
    enum Type
    {
        UPDATE,
        CULL,
        EVENT
    };

  public:
    ModelCallback(Type type);

  public:
    virtual void operator()(State& state, Model* model) = 0;

    Type getType() const;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr