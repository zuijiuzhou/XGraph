#include "Uniform.h"

#include <any>

#include <glm/glm.hpp>

#include "Shader.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(Uniform, StateAttribute);

struct Uniform::Data {
    std::string name;
    std::any    val;
    ValueType   type;
};

Uniform::Uniform()
  : d(new Data()) {
}

Uniform::Uniform(const std::string& name, bool val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = BOOL;
}
Uniform::Uniform(const std::string& name, const glm::bvec2& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = BVEC2;
}
Uniform::Uniform(const std::string& name, const glm::bvec3& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = BVEC3;
}
Uniform::Uniform(const std::string& name, const glm::bvec4& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = BVEC4;
}


Uniform::Uniform(const std::string& name, int val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = INT;
}
Uniform::Uniform(const std::string& name, const glm::ivec2& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = IVEC2;
}
Uniform::Uniform(const std::string& name, const glm::ivec3& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = IVEC3;
}
Uniform::Uniform(const std::string& name, const glm::ivec4& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = IVEC4;
}


Uniform::Uniform(const std::string& name, float val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = FLOAT;
}
Uniform::Uniform(const std::string& name, const glm::fvec2& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = FVEC2;
}
Uniform::Uniform(const std::string& name, const glm::fvec3& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = FVEC3;
}
Uniform::Uniform(const std::string& name, const glm::fvec4& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = FVEC4;
}
Uniform::Uniform(const std::string& name, const glm::fmat3x3& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = FMAT3X3;
}
Uniform::Uniform(const std::string& name, const glm::fmat4x4& mat)
  : Uniform() {
    d->name = name;
    d->val  = mat;
    d->type = FMAT4X4;
}

Uniform::Uniform(const std::string& name, double val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = DOUBLE;
}
Uniform::Uniform(const std::string& name, const glm::dvec2& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = DVEC2;
}
Uniform::Uniform(const std::string& name, const glm::dvec3& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = DVEC3;
}
Uniform::Uniform(const std::string& name, const glm::dvec4& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = DVEC4;
}
Uniform::Uniform(const std::string& name, const glm::dmat3x3& val)
  : Uniform() {
    d->name = name;
    d->val  = val;
    d->type = DMAT3X3;
}
Uniform::Uniform(const std::string& name, const glm::dmat4x4& mat)
  : Uniform() {
    d->name = name;
    d->val  = mat;
    d->type = DMAT4X4;
}

Uniform::~Uniform() {
    delete d;
}

StateAttribute::Type Uniform::getType() const {
    return Type::ATTR_UNIFORM;
}

Uniform::ValueType Uniform::getValueType() const {
    return d->type;
}

void Uniform::setValue(bool val) {
    if (d->type == BOOL) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::bvec2& val) {
    if (d->type == BVEC2) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::bvec3& val) {
    if (d->type == BVEC3) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::bvec4& val) {
    if (d->type == BVEC4) {
        d->val = val;
    }
}

void Uniform::setValue(int val) {
    if (d->type == INT) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::ivec2& val) {
    if (d->type == IVEC2) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::ivec3& val) {
    if (d->type == IVEC3) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::ivec4& val) {
    if (d->type == IVEC4) {
        d->val = val;
    }
}

void Uniform::setValue(float val) {
    if (d->type == FLOAT) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::fvec2& val) {
    if (d->type == FVEC2) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::fvec3& val) {
    if (d->type == FVEC3) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::fvec4& val) {
    if (d->type == FVEC4) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::fmat3x3& val) {
    if (d->type == FMAT3X3) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::fmat4x4& val) {
    if (d->type == FMAT4X4) {
        d->val = val;
    }
}

void Uniform::setValue(double val) {
    if (d->type == DOUBLE) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::dvec2& val) {
    if (d->type == DVEC2) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::dvec3& val) {
    if (d->type == DVEC3) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::dvec4& val) {
    if (d->type == DVEC4) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::dmat3x3& val) {
    if (d->type == DMAT3X3) {
        d->val = val;
    }
}
void Uniform::setValue(const glm::dmat4x4& val) {
    if (d->type == DMAT4X4) {
        d->val = val;
    }
}


void Uniform::apply(State& ctx) const {
    auto shader = ctx.getCurrentShader();
    if (shader) {
        switch (d->type) {
        case BOOL: shader->set(ctx, d->name, std::any_cast<bool>(d->val)); break;
        case BVEC2: shader->set(ctx, d->name, std::any_cast<glm::bvec2>(d->val)); break;
        case BVEC3: shader->set(ctx, d->name, std::any_cast<glm::bvec3>(d->val)); break;
        case BVEC4: shader->set(ctx, d->name, std::any_cast<glm::bvec4>(d->val)); break;

        case INT: shader->set(ctx, d->name, std::any_cast<int>(d->val)); break;
        case IVEC2: shader->set(ctx, d->name, std::any_cast<glm::ivec2>(d->val)); break;
        case IVEC3: shader->set(ctx, d->name, std::any_cast<glm::ivec3>(d->val)); break;
        case IVEC4: shader->set(ctx, d->name, std::any_cast<glm::ivec4>(d->val)); break;

        case FLOAT: shader->set(ctx, d->name, std::any_cast<float>(d->val)); break;
        case FVEC2: shader->set(ctx, d->name, std::any_cast<glm::fvec2>(d->val)); break;
        case FVEC3: shader->set(ctx, d->name, std::any_cast<glm::fvec3>(d->val)); break;
        case FVEC4: shader->set(ctx, d->name, std::any_cast<glm::fvec4>(d->val)); break;
        case FMAT3X3: shader->set(ctx, d->name, std::any_cast<glm::fmat3x3>(d->val)); break;
        case FMAT4X4: shader->set(ctx, d->name, std::any_cast<glm::fmat4x4>(d->val)); break;

        case DOUBLE: shader->set(ctx, d->name, std::any_cast<double>(d->val)); break;
        case DVEC2: shader->set(ctx, d->name, std::any_cast<glm::dvec2>(d->val)); break;
        case DVEC3: shader->set(ctx, d->name, std::any_cast<glm::dvec3>(d->val)); break;
        case DVEC4: shader->set(ctx, d->name, std::any_cast<glm::dvec4>(d->val)); break;
        case DMAT3X3: shader->set(ctx, d->name, std::any_cast<glm::dmat3x3>(d->val)); break;
        case DMAT4X4: shader->set(ctx, d->name, std::any_cast<glm::dmat4x4>(d->val)); break;
        }
    }
}

} // namespace glr