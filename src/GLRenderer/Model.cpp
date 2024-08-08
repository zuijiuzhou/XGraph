#include "Model.h"

#include <vine/core/Ptr.h>

#include "Drawable.h"
#include "StateSet.h"

namespace glr {
VI_OBJECT_META_IMPL(Model, Object);

struct Model::Data {
    vine::RefPtr<StateSet>                   state_set = nullptr;
    glm::mat4                                matrix    = glm::mat4(1.0);
    std::vector<vine::RefPtr<Drawable>>      drawables;
    std::vector<vine::RefPtr<ModelCallback>> callbacks;
    BoundingBox                              bb;
    bool                                     bb_is_dirty = true;
};
Model::Model()
  : d(new Data()) {
}

Model::~Model() {
    delete d;
}

glm::mat4 Model::getMatrix() const {
    return d->matrix;
}

void Model::setMatrix(const glm::mat4& mat) {
    d->matrix = mat;
}

StateSet* Model::getOrCreateStateSet() {
    if (!d->state_set.get()) {
        d->state_set = new StateSet();
    }
    return d->state_set.get();
}

StateSet* Model::getStateSet() {
    return d->state_set.get();
}

void Model::addDrawable(Drawable* drawable) {
    d->drawables.push_back(drawable);
    d->bb_is_dirty = true;
}

void Model::removeDrawable(Drawable* drawable) {
    auto found_at = std::find(d->drawables.rbegin(), d->drawables.rend(), drawable);
    if (found_at != d->drawables.rend()) {
        d->drawables.erase(found_at.base());
        d->bb_is_dirty = true;
    }
}

int Model::getNbDrawables() const {
    return d->drawables.size();
}

Drawable* Model::getDrawableAt(int index) const {
    return d->drawables.at(index).get();
}

BoundingBox Model::getBoundingBox() const {
    if (d->bb_is_dirty) {
        BoundingBox bb;
        for (auto d : d->drawables) {
            bb.combine(d->getBoundingBox());
        }
        auto this_            = const_cast<Model*>(this);
        this_->d->bb          = bb;
        this_->d->bb_is_dirty = false;
    }
    return d->bb;
}

void Model::addCallback(ModelCallback* callback) {
    auto found_at = std::find(d->callbacks.begin(), d->callbacks.end(), callback);
    if (found_at == d->callbacks.end()) {
        d->callbacks.push_back(callback);
    }
}

void Model::removeCallback(ModelCallback* callback) {
    auto found_at = std::find(d->callbacks.begin(), d->callbacks.end(), callback);
    if (found_at != d->callbacks.end()) {
        d->callbacks.erase(found_at);
    }
}

void Model::update(State& ctx) {
    for (auto& c : d->callbacks) {
        if (c->getType() == ModelCallback::UPDATE) {
            (*c)(ctx, this);
        }
    }
}

struct ModelCallback::Data {
    ModelCallback::Type type;
};

ModelCallback::ModelCallback(Type t)
  : d(new Data()) {
    d->type = t;
}

ModelCallback::Type ModelCallback::getType() const {
    return d->type;
}
} // namespace glr