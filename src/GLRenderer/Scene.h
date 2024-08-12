#pragma once
#include "Object.h"
namespace glr {
class Model;
class Scene : public Object {
    VI_OBJECT_META;
    VI_DISABLE_COPY_MOVE(Scene);

  public:
    Scene();
    virtual ~Scene();

  public:
    void addModel(Model* model);

    size_t getNbModels() const;

    Model* getModelAt(size_t i) const;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr