#pragma once

#include "glrenderer_global.h"

#include "SceneNodeGroup.h"

namespace glr {
class Drawable;
class Event;

class GLRENDERER_API Model : public SceneNodeGroup {
    VI_OBJECT_META;

  public:
    Model();
    virtual ~Model();

  public:
    virtual void addChild(SceneNode* node) override;

    void addDrawable(Drawable* drawable);
    void removeDrawable(Drawable* drawable);

    int       getNbDrawables() const;
    Drawable* getDrawableAt(int index) const;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr