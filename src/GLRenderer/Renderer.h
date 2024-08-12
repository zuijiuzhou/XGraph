#pragma once
#include "BoundingBox.h"
#include "Object.h"
#include <vector>

namespace glr {
class Shader;
class Model;
class Camera;
class CameraManipulator;
class GraphicContext;
class Scene;
class RenderInfo;

class Renderer : public Object {
    VI_OBJECT_META;

  public:
    enum RenderOrder
    {
        PRE_RENDER,
        MID_RENDER,
        POST_RENDER,
    };

  public:
    Renderer();
    virtual ~Renderer();

  public:
    void    render(RenderInfo& info);
    Camera* getCamera() const;

    void            setCamera(Camera* cam);
    GraphicContext* getContext() const;

    void               setContext(GraphicContext* ctx);
    CameraManipulator* getCameraManipulator() const;

    void        setCameraManipulator(CameraManipulator* cm);
    void        setRenderOrder(RenderOrder order);
    RenderOrder getRenderOrder() const;

    void   setScene(Scene* scene);
    Scene* getScene() const;

    void setUseMasterScene(bool val);
    bool getUseMasterScene() const;

    void setUseMasterViewport(bool val);
    bool getUseMasterViewport() const;

    void setUseMasterViewMatrix(bool val);
    bool getUseMasterViewMatrix() const;

    void setUseMasterProjectionMatrix(bool val);
    bool getUseMasterProjectionMatrix() const;

  private:
    VI_OBJECT_DATA;
};

} // namespace glr
