#include "Uniform.h"

#include "Shader.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(Uniform, StateAttribute);

struct Uniform::Data {
    std::string name;
    int         int_val;
    bool        bool_val;
    ValueType   type;
};

Uniform::Uniform()
  : d(new Data()) {
}

Uniform::Uniform(const std::string& name, int val)
  : Uniform() {
    d->name    = name;
    d->int_val = val;
    d->type    = INT;
}

Uniform::Uniform(const std::string& name, bool val)
  : Uniform() {
    d->name     = name;
    d->bool_val = val;
    d->type     = BOOL;
}

Uniform::~Uniform() {
    delete d;
}

void Uniform::apply(State& ctx) const {
    auto shader = ctx.getCurrentShader();
    if (shader) {
        switch (d->type) {
        case INT: shader->set(ctx, d->name, d->int_val); break;
        case BOOL: shader->set(ctx, d->name, d->bool_val); break;
        }
    }
}

StateAttribute::Type Uniform::getType() const {
    return Type::ATTR_UNIFORM;
}
} // namespace glr