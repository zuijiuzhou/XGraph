#pragma once

#include "glrenderer_global.h"

#include "SceneNodeGroup.h"

namespace glr {
class Model;
class GLRENDERER_API Scene : public SceneNodeGroup {
    VI_OBJECT_META;

  public:
    Scene();
    virtual ~Scene();

  public:

  private:
    VI_OBJECT_DATA;
};
} // namespace glr