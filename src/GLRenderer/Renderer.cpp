#include "Renderer.h"
#include <functional>
#include <glad/glad.h>
#include <iostream>

#include <vine/core/Ptr.h>

#include "StdUtils/Resources.h"

#include "Camera.h"
#include "CameraManipulator.h"
#include "Drawable.h"
#include "GraphicContext.h"
#include "Model.h"
#include "Shader.h"
#include "State.h"
#include "StateSet.h"

namespace glr {
VI_OBJECT_META_IMPL(Renderer, Object);

extern void state_set_current_camera(void* data, Camera* cam);

struct Renderer::Data {
    vine::RefPtr<Camera>              camera;
    vine::RefPtr<GraphicContext>      ctx;
    vine::RefPtr<CameraManipulator>   cm;
    std::vector<vine::RefPtr<Model>>  models;
    std::vector<vine::RefPtr<Shader>> shaders;
    BoundingBox                       bb;
    bool                              is_first_frame = true;
};

Renderer::Renderer()
  : d(new Data()) {
    auto cam  = new Camera();
    d->camera = cam;
}

Renderer::~Renderer() {
    delete d;
}

void Renderer::addModel(Model* model) {
    if (std::find(d->models.begin(), d->models.end(), model) == d->models.end()) {
        d->models.push_back(model);
    }
}

Camera* Renderer::getCamera() const {
    return d->camera.get();
}

void Renderer::setCamera(Camera* cam) {
    d->camera = cam;
}

GraphicContext* Renderer::getContext() const {
    return d->ctx.get();
}

void Renderer::setContext(GraphicContext* ctx) {
    d->ctx = ctx;
}

CameraManipulator* Renderer::getCameraManipulator() const {
    return d->cm.get();
}

void Renderer::setCameraManipulator(CameraManipulator* cm) {
    d->cm = cm;
}

void Renderer::render() {
    if (!d->ctx) return;
    if (d->is_first_frame) {
        d->is_first_frame = false;
    }
    d->ctx->makeCurrent();
    d->camera->apply();

    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    auto  matrix_v  = d->camera->getViewMatrix();
    auto  matrix_vp = d->camera->getViewProjectionMatrix();
    auto  view_dir  = d->camera->getViewDir();
    auto& ctx       = *d->ctx;
    auto& state     = *ctx.getState();
    for (auto model : d->models) {
        state_set_current_camera(state.d, d->camera.get());
        auto stateset = model->getStateSet();
        if (stateset) {
            state.applyShader(stateset);
        }
        model->update(state);
        auto matrix_m = model->getMatrix();
        if (stateset) {
            state.applyAttributes(stateset);
            auto shader = state.getCurrentShader();
            if (shader) {
                shader->set(state, "matrix_m", matrix_m);
                shader->set(state, "matrix_v", matrix_v);
                shader->set(state, "matrix_mv", matrix_v * matrix_m);
                shader->set(state, "matrix_mvp", matrix_vp * matrix_m);
                shader->set(state, "view_dir", view_dir);
            }
        }
        for (int i = 0; i < model->getNbDrawables(); i++) {
            auto drawable = model->getDrawableAt(i);
            drawable->draw(state);
        }
        if (stateset) {
            state.restoreAttributes(stateset);
        }
    }
}

} // namespace glr