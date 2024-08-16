#include "Scene.h"

#include <vector>

#include "Model.h"

namespace glr {
VI_OBJECT_META_IMPL(Scene, EventReceiver);

struct Scene::Data { 
};

Scene::Scene()
  : d(new Data()) {
}

Scene::~Scene() {
    delete d;
}

} // namespace glr