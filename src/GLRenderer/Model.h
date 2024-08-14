#pragma once
#include <glm/mat4x4.hpp>

#include "BoundingBox.h"
#include "Object.h"

#include "EventReceiver.h"

namespace glr {
class Drawable;
class StateSet;
class State;
class UpdateCallback;
class Event;

class Model : public EventReceiver {
    VI_OBJECT_META;

  public:
    Model();
    virtual ~Model();

  public:
    glm::mat4 getMatrix() const;
    void      setMatrix(const glm::mat4& mat);

    StateSet* getOrCreateStateSet();
    StateSet* getStateSet();

    void addDrawable(Drawable* drawable);
    void removeDrawable(Drawable* drawable);

    int       getNbDrawables() const;
    Drawable* getDrawableAt(int index) const;

    BoundingBox getBoundingBox() const;

    virtual bool handleEvent(Event* e) override;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr