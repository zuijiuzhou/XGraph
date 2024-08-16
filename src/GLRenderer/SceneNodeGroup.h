
#pragma once

#include "glrenderer_global.h"

#include "SceneNode.h"

namespace glr {
class StateSet;

class GLRENDERER_API SceneNodeGroup : public SceneNode {
    VI_OBJECT_META;

  public:
    SceneNodeGroup();
    ~SceneNodeGroup();

  public:
     virtual void addChild(SceneNode* node);
     virtual void removeChild(SceneNode* node);
     int getNbChildren() const;
     SceneNode* getChildAt(int idx) const;

    virtual bool handleEvent(Event* e) override;
    virtual void update(UpdateContext* ctx) override;

  protected:
    virtual void onComputeBoundingBox(BoundingBox& bb) const override;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr