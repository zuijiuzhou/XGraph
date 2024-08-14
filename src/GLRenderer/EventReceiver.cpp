#include "EventReceiver.h"

#include <vector>

#include "Event.h"
#include "Callbacks.h"

namespace glr {

VI_OBJECT_META_IMPL(EventReceiver, Object);

struct EventReceiver::Data {
    std::vector<vine::RefPtr<EventHandler>> event_handlers;
    std::vector<vine::RefPtr<UpdateCallback>> update_callbacks;
};

EventReceiver::EventReceiver()
  : d(new Data()) {
}

EventReceiver::~EventReceiver() {
    delete d;
}

bool EventReceiver::handleEvent(Event* e) {
    auto handled = false;
    for (auto& handler : d->event_handlers) {
        handled |= handler->handle(e);
    }
    return handled;
}

void EventReceiver::addEventHandler(EventHandler* handler) {
    auto found_at = std::find(d->event_handlers.begin(), d->event_handlers.end(), handler);
    if (found_at == d->event_handlers.end()) {
        d->event_handlers.push_back(handler);
    }
}

void EventReceiver::removeEventHandler(EventHandler* handler) {
    auto found_at = std::find(d->event_handlers.begin(), d->event_handlers.end(), handler);
    if (found_at != d->event_handlers.end()) {
        d->event_handlers.erase(found_at);
    }
}

void EventReceiver::update(UpdateContext* ctx) {
    for (auto& callback : d->update_callbacks) {
        callback->operator()(this, ctx);
    }
}

void EventReceiver::addUpdateCallback(UpdateCallback* callback) {
     auto found_at = std::find(d->update_callbacks.begin(), d->update_callbacks.end(), callback);
    if (found_at == d->update_callbacks.end()) {
        d->update_callbacks.push_back(callback);
    }
}

void EventReceiver::removeUpdateCallback(UpdateCallback* callback) {
    auto found_at = std::find(d->update_callbacks.begin(), d->update_callbacks.end(), callback);
    if (found_at != d->update_callbacks.end()) {
        d->update_callbacks.erase(found_at);
    }
}
} // namespace glr