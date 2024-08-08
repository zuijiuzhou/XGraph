#pragma once
#include "Object.h"

namespace glr {
class GLObject;
class Shader;
class Camera;
class GraphicContext;
class Renderer;
class StateSet;
class State : public Object {
    VI_OBJECT_META;

    friend class Renderer;

  public:
    State(GraphicContext* ctx);
    virtual ~State();

  public:
    Shader*         getCurrentShader() const;
    Renderer*       getCurrentRenderer() const;
    Camera*         getCurrentCamera() const;
    GraphicContext* getContext() const;

    void applyShader(StateSet* ss);
    void applyAttributes(StateSet* ss);
    void restoreAttributes(StateSet* ss);

    void attachGLObject(GLObject* obj);
    void detachGLObject(GLObject* obj);
    void releaseGLObjects();

  private:
    VI_OBJECT_DATA;
};
} // namespace glr