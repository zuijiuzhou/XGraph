#include "GraphicContext.h"

#include <queue>

#include <vine/core/Ptr.h>

#include "GLVersion.h"
#include "ResourceManager.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(GraphicContext, Object);

struct GraphicContext::EventQueue::Data {
    std::queue<vine::RefPtr<Event>> events;
};

GraphicContext::EventQueue::EventQueue()
  : d(new Data()) {
}

void GraphicContext::EventQueue::push(Event* e) {
    d->events.push(e);
}

Event* GraphicContext::EventQueue::pop() {
    if (d->events.size()) {
        auto e = d->events.front();
        d->events.pop();
        return e.release();
    }
    return nullptr;
}

void GraphicContext::EventQueue::clear() {
    d->events = std::queue<vine::RefPtr<Event>>();
}

int GraphicContext::EventQueue::size() const {
    return static_cast<int>(d->events.size());
}

struct GraphicContext::Data {
    static int               max_id;
    int                      id             = 0;
    bool                     is_initialized = false;
    vine::RefPtr<State>      state;
    vine::RefPtr<EventQueue> events;
    GLVersionInfo            gl_ver;
};

int GraphicContext::Data::max_id = 0;

GraphicContext::GraphicContext()
  : d(new Data()) {
    d->id     = ++Data::max_id;
    d->state  = new State(this);
    d->events = new EventQueue();
}

GraphicContext::~GraphicContext() {
    delete d;
}

int GraphicContext::getId() const {
    return d->id;
}

void GraphicContext::makeCurrent() {
}

State* GraphicContext::getState() const {
    return d->state.get();
}

GraphicContext* GraphicContext::getContextById(int id) {
    return nullptr;
}

void GraphicContext::realize() {
    if (d->is_initialized) return;



    d->is_initialized = true;
}

bool GraphicContext::isRealized() const {
    return d->is_initialized;
}

void GraphicContext::notify(Event* e) {
    d->events->push(e);
}

void GraphicContext::swapBuffers() {
}

GraphicContext::EventQueue* GraphicContext::getEventQueue() const {
    return d->events.get();
}

const GLVersionInfo& GraphicContext::getGLVersion() const {
    return d->gl_ver;
}
} // namespace glr