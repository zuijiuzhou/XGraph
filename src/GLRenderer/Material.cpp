#include "Material.h"

#include "Shader.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(Material, StateAttribute);

Material::Material()
  : a_(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f))
  , d_(glm::vec4(0.8f, 0.8f, 0.8f, 1.0f))
  , s_(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
  , e_(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
  , sh_(32.0f) {
}

glm::vec4 Material::getAmbient() const {
    return a_;
}

void Material::setAmbient(const glm::vec4& color) {
    a_ = color;
}

glm::vec4 Material::getDiffuse() const {
    return d_;
}

void Material::setDiffuse(const glm::vec4& color) {
    d_ = color;
}

glm::vec4 Material::getSpecular() const {
    return s_;
}

void Material::setSpecular(const glm::vec4& color) {
    s_ = color;
}

glm::vec4 Material::getEmission() const {
    return e_;
}

void Material::setEmission(const glm::vec4& color) {
    e_ = color;
}

float Material::getShininess() const {
    return sh_;
}

void Material::setShininess(float shininess) {
    sh_ = shininess;
}

StateAttribute::Type Material::getType() const {
    return ATTR_MATERIAL;
}

void Material::apply(State& state) const {
    auto shader = state.getCurrentShader();
    if (shader) {
        shader->set(state, "mate.a", a_);
        shader->set(state, "mate.d", d_);
        shader->set(state, "mate.s", s_);
        shader->set(state, "mate.e", e_);
        shader->set(state, "mate.sh", sh_);
    }
}

} // namespace glr