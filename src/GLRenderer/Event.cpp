#include "Event.h"
#include "GraphicContext.h"

namespace glr {
VI_OBJECT_META_IMPL(Event, Object);

struct Event::Data {
    GraphicContext* ctx;
    EventType       type;
    MouseButton     mouse_btn   = ButtonNone;
    int             mouse_x     = 0;
    int             mouse_y     = 0;
    int             mouse_delta = 0;
    int             size_width  = 0;
    int             size_height = 0;
};

Event::Event(GraphicContext* ctx, EventType type)
  : d(new Data()) {
    d->ctx  = ctx;
    d->type = type;
}
EventType Event::getType() const {
    return d->type;
}

int Event::getWidth() const {
    return d->size_width;
}

int Event::getHeight() const {
    return d->size_height;
}

int Event::getMouseX() const {
    return d->mouse_x;
}

int Event::getMouseY() const {
    return d->mouse_y;
}

MouseButton Event::getMouseButton() const {
    return d->mouse_btn;
}

int Event::getMouseDelta() const {
    return d->mouse_delta;
}

GraphicContext* Event::getContext() const {
    return d->ctx;
}

Event* Event::createMousePressEvent(GraphicContext* ctx, MouseButton button, int x, int y) {
    auto e          = new Event(ctx, MousePress);
    e->d->mouse_btn = button;
    e->d->mouse_x   = x;
    e->d->mouse_y   = y;
    return e;
}
Event* Event::createMouseMoveEvent(GraphicContext* ctx, int x, int y) {
    auto e        = new Event(ctx, MouseMove);
    e->d->mouse_x = x;
    e->d->mouse_y = y;
    return e;
}
Event* Event::createMouseReleaseEvent(GraphicContext* ctx, MouseButton button, int x, int y) {
    auto e          = new Event(ctx, MouseRelease);
    e->d->mouse_btn = button;
    e->d->mouse_x   = x;
    e->d->mouse_y   = y;
    return e;
}
Event* Event::createMouseWheelEvent(GraphicContext* ctx, int delta) {
    auto e            = new Event(ctx, MouseWheel);
    e->d->mouse_delta = delta;
    return e;
}
Event* Event::createResizeEvent(GraphicContext* ctx, int w, int h) {
    auto e            = new Event(ctx, Resize);
    e->d->size_width  = w;
    e->d->size_height = h;
    return e;
}
} // namespace glr