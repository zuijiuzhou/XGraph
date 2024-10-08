#include "CameraManipulator.h"

#include <functional>

#include <vine/core/Ptr.h>

#include "Camera.h"

namespace glr {
VI_OBJECT_META_IMPL(CameraManipulator, Object);
VI_OBJECT_META_IMPL(StandardCameraManipulator, CameraManipulator);

struct StandardCameraManipulator::Data {
    vine::RefPtr<Camera> camera;
    bool                 is_rotation_started    = false;
    bool                 is_pan_started         = false;
    bool                 is_cursor_move_started = false;
    int                  width = 1, height = 1.;
    int                  vx = 0, vy = 0, vw = 1, vh = 1;
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

bool StandardCameraManipulator::handleEvent(Event* e) {
    switch (e->getType()) {
    case EventType::MousePress:
    {
        handleMousePressed(e->getMouseButton(), e->getMouseX(), e->getMouseY());
        break;
    }
    case EventType::MouseMove:
    {
        handleMouseMoved(e->getMouseX(), e->getMouseY());
        break;
    }
    case EventType::MouseRelease:
    {
        handleMouseReleased(e->getMouseButton(), e->getMouseX(), e->getMouseY());
        break;
    }
    case EventType::MouseWheel:
    {
        handleMouseScrolled(e->getMouseDelta());
        break;
    }
    case EventType::Resize:
    {
        handleResized(e->getWidth(), e->getHeight());
        break;
    }
    default: break;
    }
    return false;
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

bool StandardCameraManipulator::onUpdateViewport(int w, int h, int& vx, int& vy, int& vw, int& vh)
{
    vx = 0;
    vy = 0;
    vw = w;
    vh = h;
    return true;
}

glm::mat4 StandardCameraManipulator::computeViewMatrix() const {
    return glm::lookAt<float>(d->eye, d->target, d->up);
}

glm::mat4 StandardCameraManipulator::computeProjectionMatrix() const {
    if (d->proj_type == Perspective)
        return glm::perspective<double>(glm::radians<double>(d->fov), ((double)d->vw) / d->vh, d->near, d->far);
    else
        return glm::ortho<double>(-d->vw / 2., d->vw / 2., -d->vh / 2., d->vh / 2.);
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

    int vx = 0, vy = 0, vw = w, vh = h;

    if(onUpdateViewport(w, h, vx, vy, vw, vh)){
        d->vx = vx;
        d->vy = vy;
        d->vw = vw;
        d->vh = vh;
        d->camera->setViewport(vx, vy, vw, vh);
        d->camera->setProjectionMatrix(computeProjectionMatrix());
    }
}
} // namespace glr