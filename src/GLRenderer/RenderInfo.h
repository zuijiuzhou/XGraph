#pragma once
#include "Object.h"

namespace glr {

class Scene;
class Viewer;

class RenderInfo : public Object {
    VI_OBJECT_META;
    VI_DISABLE_COPY_MOVE(RenderInfo);

  public:
    RenderInfo(Viewer* viewer);
    virtual ~RenderInfo();

  public:
    Viewer* getViewer() const;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr