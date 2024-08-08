#pragma once
#include "Event.h"
#include "Object.h"
#include <glm/mat4x4.hpp>

namespace glr {
class Camera;
class CameraManipulator : public Object {
    VI_OBJECT_META;

  public:
    enum ProjectionType
    {
        Perspective,
        Orthographic
    };

    enum ViewMode
    {
        Top,
        Bottom,
        Left,
        Right,
        Front,
        Back
    };

  public:
    virtual void handleEvent(Event* e) = 0;

    virtual ProjectionType getProjectionType() const              = 0;
    virtual void           setProjectionType(ProjectionType type) = 0;

    virtual double getFov() const     = 0;
    virtual void   setFov(double fov) = 0;

    virtual void getViewAsLookAt(glm::vec3& eye, glm::vec3& target, glm::vec3& up)                   = 0;
    virtual void setViewAsLookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) = 0;

    virtual void setViewMode(ViewMode mode) = 0;
};

class StandardCameraManipulator final : public CameraManipulator {
    VI_OBJECT_META;

  public:
    StandardCameraManipulator(Camera* cam);
    virtual ~StandardCameraManipulator();

  public:
    virtual void handleEvent(Event* e) override;

    virtual ProjectionType getProjectionType() const override;
    virtual void           setProjectionType(ProjectionType type) override;

    virtual double getFov() const override;
    virtual void   setFov(double fov) override;

    virtual void getViewAsLookAt(glm::vec3& eye, glm::vec3& target, glm::vec3& up) override;
    virtual void setViewAsLookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) override;

    virtual void setViewMode(ViewMode mode) override;

  private:
    virtual glm::mat4 computeViewMatrix() const;
    virtual glm::mat4 computeProjectionMatrix() const;

    void handleMousePressed(MouseButton btn, int x, int y);
    void handleMouseReleased(MouseButton btn, int x, int y);
    void handleMouseMoved(int x, int y);
    void handleMouseScrolled(int delta);
    void handleResized(int w, int h);

  private:
    VI_OBJECT_DATA;
};

} // namespace glr
