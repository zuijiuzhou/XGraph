#include "CameraManipulator.h"

#include <vine/core/Ptr.h>

#include "Camera.h"
#include <functional>

namespace glr {
VI_OBJECT_META_IMPL(CameraManipulator, Object);
VI_OBJECT_META_IMPL(StandardCameraManipulator, CameraManipulator);

struct StandardCameraManipulator::Data {
    vine::RefPtr<Camera> camera;
    bool                 is_rotation_started    = false;
    bool                 is_pan_started         = false;
    bool                 is_cursor_move_started = false;
    int                  width = 1, height = 1.;
    double               near = 1., far = 1000., fov = 30.;
    ProjectionType       proj_type = Perspective;
    glm::vec2            prev_cursor_pt;
    glm::vec3            eye, target, up;
};

StandardCameraManipulator::StandardCameraManipulator(Camera* cam)
  : d(new Data()) {
    d->camera = cam;
}

StandardCameraManipulator::~StandardCameraManipulator() {
    delete d;
}

void StandardCameraManipulator::handleEvent(Event* e) {
    switch (e->getType()) {
    case EventType::MousePress: handleMousePressed(e->getMouseButton(), e->getMouseX(), e->getMouseY()); break;

    case EventType::MouseMove: handleMouseMoved(e->getMouseX(), e->getMouseY()); break;

    case EventType::MouseRelease: handleMouseReleased(e->getMouseButton(), e->getMouseX(), e->getMouseY()); break;

    case EventType::MouseWheel: handleMouseScrolled(e->getMouseDelta()); break;

    case EventType::Resize: handleResized(e->getWidth(), e->getHeight()); break;
    default: break;
    }
}

void StandardCameraManipulator::handleMouseReleased(MouseButton btn, int x, int y) {
    d->is_pan_started         = false;
    d->is_rotation_started    = false;
    d->is_cursor_move_started = false;
}

void StandardCameraManipulator::handleMousePressed(MouseButton btn, int x, int y) {
    switch (btn) {
    case MouseButton::ButtonLeft:
    {
        d->is_rotation_started = true;
    } break;
    case MouseButton::ButtonMiddle:
    {
        d->is_pan_started = true;
    } break;
    case MouseButton::ButtonRight:
    {
    } break;
    default: break;
    }
}

void StandardCameraManipulator::handleMouseMoved(int x, int y) {
    auto xx = static_cast<float>(x);
    auto yy = static_cast<float>(y);
    if (d->is_cursor_move_started) {
        auto delta_x = xx - d->prev_cursor_pt.x;
        auto delta_y = yy - d->prev_cursor_pt.y;

        auto vm = d->camera->getViewMatrix();
        if (d->is_pan_started) {
            auto m = glm::mat4(1.0f);
            m      = glm::translate(m, glm::vec3(delta_x / 500, -delta_y / 500, 0));
            vm     = m * vm;
            d->camera->setViewMatrix(vm);
        }
        if (d->is_rotation_started) {
            glm::mat4 mx(1.0f);
            glm::mat4 my(1.0f);

            mx = glm::rotate(mx, glm::radians(delta_x / 10), glm::vec3(0.0f, 1.0f, 0.0f));
            my = glm::rotate(my, glm::radians(delta_y / 10), glm::vec3(1.0f, 0.0f, 0.0f));

            vm *= mx;
            vm *= my;

            d->camera->setViewMatrix(vm);
        }
        d->prev_cursor_pt.x = xx;
        d->prev_cursor_pt.y = yy;
    }
    else {
        d->is_cursor_move_started = true;
        d->prev_cursor_pt.x       = xx;
        d->prev_cursor_pt.y       = yy;
    }
}

void StandardCameraManipulator::handleMouseScrolled(int delta) {
    auto      vm = d->camera->getViewMatrix();
    glm::vec3 p, t, u, dir;
    d->camera->getViewMatrixAsLookAt(p, t, u);
    dir = t - p;
    dir = dir * (delta / 10.f);
    glm::mat4 m(1.0);
    m = glm::translate(m, dir);
    vm *= m;
    d->camera->setViewMatrix(vm);
}

void StandardCameraManipulator::handleResized(int w, int h) {
    d->width  = w;
    d->height = h;
    d->camera->setViewport(0, 0, w, h);
    d->camera->setProjectionMatrix(computeProjectionMatrix());
}

CameraManipulator::ProjectionType StandardCameraManipulator::getProjectionType() const {
    return d->proj_type;
};

void StandardCameraManipulator::setProjectionType(ProjectionType type) {
    d->proj_type = type;
};

double StandardCameraManipulator::getFov() const {
    return d->fov;
};

void StandardCameraManipulator::setFov(double fov) {
    d->fov = fov;
};

void StandardCameraManipulator::getViewAsLookAt(glm::vec3& eye, glm::vec3& target, glm::vec3& up) {
    eye    = d->eye;
    target = d->target;
    up     = d->up;
};

void StandardCameraManipulator::setViewAsLookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up) {
    d->eye    = eye;
    d->target = target;
    d->up     = up;
    d->camera->setViewMatrix(computeViewMatrix());
};

void StandardCameraManipulator::setViewMode(ViewMode mode) {

};

glm::mat4 StandardCameraManipulator::computeViewMatrix() const {
    return glm::lookAt<float>(d->eye, d->target, d->up);
}

glm::mat4 StandardCameraManipulator::computeProjectionMatrix() const {
    if (d->proj_type == Perspective)
        return glm::perspective<double>(glm::radians<double>(d->fov), ((double)d->width) / d->height, d->near, d->far);
    else
        return glm::ortho<double>(-d->width / 2., d->width / 2., -d->height / 2., d->height / 2.);
}
} // namespace glr