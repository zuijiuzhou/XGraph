#pragma once

#include "glrenderer_global.h"

#include <vector>

#include <glm/glm.hpp>

#include "BoundingBox.h"
#include "StateSet.h"

namespace glr {

class SceneNode;
class SceneNodePrivate {
    friend class SceneNode;

  private:
    std::vector<SceneNode*> parent_nodes;
    vine::RefPtr<StateSet>  state_set = nullptr;
    glm::mat4               matrix    = glm::mat4(1.0);
    BoundingBox             bb;
    bool                    bound_is_dirty = true;

  public:
    void addParent(SceneNode* node);
    void removeParent(SceneNode* node);
};
} // namespace glr