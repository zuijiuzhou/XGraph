#include "Scene.h"

#include <vector>

#include "Model.h"

namespace glr {
VI_OBJECT_META_IMPL(Scene, EventReceiver);

struct Scene::Data {
    std::vector<vine::RefPtr<Model>> models;
};

Scene::Scene()
  : d(new Data()) {
}

Scene::~Scene() {
    delete d;
}


void Scene::addModel(Model* model) {
    if (std::find(d->models.begin(), d->models.end(), model) == d->models.end()) {
        d->models.push_back(model);
    }
}

size_t Scene::getNbModels() const {
    return d->models.size();
}

Model* Scene::getModelAt(size_t i) const {
    return d->models.at(i).get();
}


bool Scene::handleEvent(Event* e) {
    auto handled = EventReceiver::handleEvent(e);

    for (auto& model : d->models) {
        handled |= model->handleEvent(e);
        if (handled) break;
    }

    return handled;
}

void Scene::update(UpdateContext* ctx) {
    EventReceiver::update(ctx);
    for (auto& model : d->models) {
        model->update(ctx);
    }
}

} // namespace glr