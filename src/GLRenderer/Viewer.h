#pragma once

#include "glrenderer_global.h"

#include "Object.h"

namespace glr {
class Renderer;
class Scene;
class StateSet;

class GLRENDERER_API Viewer : public Object {
    VI_OBJECT_META;

  public:
    Viewer();
    virtual ~Viewer();

  public:
    void      frame();
    int       run();
    void      setMasterRenderer(Renderer* renderer);
    Renderer* getMasterRenderer() const;
    void      addRenderer(Renderer* renderer);
    int       getNbRenderers() const;
    Renderer* getRendererAt(int idx) const;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr