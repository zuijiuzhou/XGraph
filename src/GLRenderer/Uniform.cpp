#include "Uniform.h"

#include "Shader.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(Uniform, StateAttribute);

struct Uniform::Data {
    std::string name;
    int         val_int;
    bool        val_bool;
    glm::mat4x4 val_mat4x4;
    ValueType   type;
};

Uniform::Uniform()
  : d(new Data()) {
}

Uniform::Uniform(const std::string& name, int val)
  : Uniform() {
    d->name    = name;
    d->val_int = val;
    d->type    = INT;
}

Uniform::Uniform(const std::string& name, bool val)
  : Uniform() {
    d->name     = name;
    d->val_bool = val;
    d->type     = BOOL;
}
Uniform::Uniform(const std::string& name, const glm::mat4x4& mat)
  : Uniform() {
    d->name       = name;
    d->val_mat4x4 = mat;
    d->type       = MAT4X4;
}

Uniform::~Uniform() {
    delete d;
}

void Uniform::apply(State& ctx) const {
    auto shader = ctx.getCurrentShader();
    if (shader) {
        switch (d->type) {
        case INT: shader->set(ctx, d->name, d->val_int); break;
        case BOOL: shader->set(ctx, d->name, d->val_bool); break;
        case MAT4X4: shader->set(ctx, d->name, d->val_mat4x4); break;
        }
    }
}

StateAttribute::Type Uniform::getType() const {
    return Type::ATTR_UNIFORM;
}


void Uniform::setMat4x4(const glm::mat4x4& mat) {
  if(d->type == MAT4X4){
    d->val_mat4x4 = mat;
  }
}
} // namespace glr