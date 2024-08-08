#include "StateSet.h"
#include <vector>

#include <vine/core/Ptr.h>

#include "Shader.h"
#include "StateAttribute.h"

namespace glr {
namespace {
struct StateAttr {
    vine::RefPtr<StateAttribute> attr;

    StateAttr(StateAttribute* sa) { attr = sa; }

    bool operator==(StateAttribute* sa) const { return attr.get() == sa || attr.get()->getType() == sa->getType(); }

    bool operator!=(StateAttribute* sa) { return !(*this == sa); }
};
} // namespace

VI_OBJECT_META_IMPL(StateSet, Object);

struct StateSet::Data {
    std::vector<StateAttr> attrs;

    vine::RefPtr<Shader> shader;
};

StateSet::StateSet()
  : d(new Data()) {
}
StateSet::~StateSet() {
    delete d;
}

void StateSet::setAttribute(StateAttribute* attr) {
    if (!attr) return;
    auto found_at = std::find(d->attrs.begin(), d->attrs.end(), attr);
    if (found_at == d->attrs.end()) {
        d->attrs.emplace_back(attr);
    }
    else {
        found_at->attr = attr;
    }
}

StateAttribute* StateSet::getAttribute(StateAttribute::Type type) const {
    auto found_at = std::find_if(d->attrs.begin(), d->attrs.end(), [type](const auto& item) {
        return type == item.attr.get()->getType();
    });
    if (found_at != d->attrs.end()) {
        return found_at->attr.get();
    }
    return nullptr;
}

size_t StateSet::getNbAttributes() const {
    return d->attrs.size();
}
StateAttribute* StateSet::getAttributeAt(size_t i) {
    return (*(d->attrs.begin() + i)).attr.get();
}

Shader* StateSet::getShader() const {
    return d->shader.get();
}

void StateSet::setShader(Shader* shader) {
    if (d->shader == shader) return;
    d->shader = shader;
}
} // namespace glr