#include "Depth.h"
namespace glr {
VI_OBJECT_META_IMPL(Depth, StateAttribute);

struct Depth::Data {
    GLdouble  near = 0.0;
    GLdouble  far  = 1.0;
    GLint     func = LESS;
    GLboolean mask = true;

    GLdouble  prev_near_far[2];
    GLint     prev_func = 0;
    GLboolean prev_mask = 0;
};

Depth::Depth(double near, double far, Func func, bool mask)
  : d(new Data()) {
    d->near = near;
    d->far  = far;
    d->func = func;
    d->mask = mask;
}

Depth::Type Depth::getType() const {
    return ATTR_DEPTH;
}

void Depth::apply(State& state) const {
    glGetDoublev(GL_DEPTH_RANGE, d->prev_near_far);
    glGetBooleanv(GL_DEPTH_WRITEMASK, &d->prev_mask);
    glGetIntegerv(GL_DEPTH_FUNC, &d->prev_func);

    glDepthFunc(d->func);
    glDepthMask(d->mask);
    glDepthRange(d->near, d->far);
}

void Depth::restore(State& state) const {
    if (d->prev_func != GL_LESS) {
        int x = 1;
    }
    if (d->prev_mask != GL_TRUE) {
        int y = 2;
    }
    glDepthFunc(d->prev_func);
    glDepthMask(d->prev_mask);
    glDepthRange(d->prev_near_far[0], d->prev_near_far[1]);
}
} // namespace glr