#pragma once

#include "glrenderer_global.h"

#include "SceneNodeGroup.h"

namespace glr {
class Model;
class GLRENDERER_API Scene : public SceneNodeGroup {
    VI_OBJECT_META;
    VI_DISABLE_COPY_MOVE(Scene);

  public:
    Scene();
    virtual ~Scene();

  public:

  private:
    VI_OBJECT_DATA;
};
} // namespace glr