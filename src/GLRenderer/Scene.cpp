#include "Scene.h"

#include <vector>

#include "Model.h"

namespace glr {
VI_OBJECT_META_IMPL(Scene, Object);

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
} // namespace glr