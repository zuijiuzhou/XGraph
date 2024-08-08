
#include "Buffer.h"
#include "GraphicContext.h"
#include "State.h"
#include <map>

namespace glr {
VI_OBJECT_META_IMPL(Buffer, GLObject);

struct Buffer::Data {
    std::map<int, bool> dirties;
};

Buffer::Buffer()
  : d(new Data()) {
}

void Buffer::update(State& state) {
    auto ctx_id = state.getContext()->getId();
    if (isDirty(state)) {
        auto status        = onUpdate(state);
        d->dirties[ctx_id] = status;
    }
}

void Buffer::bind(State& state) {
    if (!isCreated(state)) create(state);
    if (!isCreated(state)) return;
    if (isDirty(state)) update(state);
    onBind(state);
}

void Buffer::unbind(State& state) {
    if (!isCreated(state)) return;
    onUnbind(state);
}

bool Buffer::isDirty(State& state) const {
    auto ctx_id = state.getContext()->getId();
    if (d->dirties.contains(ctx_id)) {
        return d->dirties[ctx_id];
    }
    return false;
}

void Buffer::dirty() {
    for (auto& kv : d->dirties) {
        kv.second = true;
    }
}
} // namespace glr