#pragma once

#include "glrenderer_global.h"

#include <glm/mat4x4.hpp>

#include "BoundingBox.h"
#include "EventReceiver.h"

namespace glr {
class StateSet;
class SceneNodeGroup;
class SceneNodePrivate;
class GLRENDERER_API SceneNode : public EventReceiver {
    VI_OBJECT_META;
    friend class SceneNodeGroup;

  public:
    SceneNode();
    ~SceneNode();

  public:
    glm::mat4 getMatrix() const;
    void      setMatrix(const glm::mat4& mat);

    StateSet* getOrCreateStateSet();
    StateSet* getStateSet();

    int getNbParents() const;
    SceneNode* getParentAt(int idx) const;

    BoundingBox getBoundingBox() const;

    void setBoundingBox(const BoundingBox& bb);

    void dirtyBound();

  protected:
    virtual void onComputeBoundingBox(BoundingBox& bb) const;

  private:
    SceneNodePrivate* const d;
};
} // namespace glr