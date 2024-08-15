#pragma once

#include "glrenderer_global.h"

#include "EventReceiver.h"

namespace glr {
class Model;
class GLRENDERER_API Scene : public EventReceiver {
    VI_OBJECT_META;
    VI_DISABLE_COPY_MOVE(Scene);

  public:
    Scene();
    virtual ~Scene();

  public:
    void addModel(Model* model);

    size_t getNbModels() const;

    Model* getModelAt(size_t i) const;

    virtual bool handleEvent(Event* e) override;
    virtual void update(UpdateContext* ctx) override;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr