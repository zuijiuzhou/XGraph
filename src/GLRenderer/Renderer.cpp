#include "Renderer.h"

#include <functional>
#include <iostream>

#include <glad/glad.h>

#include <vine/core/Ptr.h>

#include "StdUtils/Resources.h"

#include "Camera.h"
#include "CameraManipulator.h"
#include "Drawable.h"
#include "GraphicContext.h"
#include "Model.h"
#include "RenderInfo.h"
#include "Scene.h"
#include "Shader.h"
#include "State.h"
#include "StateSet.h"
#include "Viewer.h"

namespace glr {
VI_OBJECT_META_IMPL(Renderer, Object);

extern void state_set_current_camera(void* data, Camera* cam);

struct Renderer::Data {
    vine::RefPtr<Camera>            camera;
    vine::RefPtr<GraphicContext>    ctx;
    vine::RefPtr<CameraManipulator> cm;
    vine::RefPtr<Scene>             scene;
    RenderOrder                     render_order;
    bool                            use_master_scene       = false;
    bool                            use_master_viewport    = false;
    bool                            use_master_view_matrix = false;
    bool                            use_master_proj_matrix = false;
    bool                            is_first_frame         = true;
};

Renderer::Renderer()
  : d(new Data()) {
    auto cam  = new Camera();
    d->camera = cam;
}

Renderer::~Renderer() {
    delete d;
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

void Renderer::render(RenderInfo& info) {
    if (!d->ctx) return;
    if (d->is_first_frame) {
        d->is_first_frame = false;
    }
    d->ctx->makeCurrent();

    glDepthRange(0.0, 1.0);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    auto master_renderer = info.getMasterRenderer();
    auto master_cam      = master_renderer->getCamera();


    if (d->use_master_viewport) {
        master_cam->applyViewport();
        d->camera->applyAllExceptViewport();
    }
    else {
        d->camera->apply();
    }

    auto  matrix_v  = d->use_master_view_matrix ? master_cam->getViewMatrix() : d->camera->getViewMatrix();
    auto  matrix_p  = d->use_master_proj_matrix ? master_cam->getProjectionMatrix() : d->camera->getProjectionMatrix();
    auto  matrix_vp = matrix_p * matrix_v;
    auto  view_dir  = d->use_master_view_matrix ? master_cam->getViewDir() : d->camera->getViewDir();
    auto& ctx       = *d->ctx;
    auto& state     = *ctx.getState();

    auto scene = d->use_master_scene ? master_renderer->getScene() : d->scene;
    if (!scene) {
        return;
    }

    auto nb_models = scene->getNbModels();
    for (size_t i = 0; i < nb_models; ++i) {
        auto model = scene->getModelAt(i);
        state_set_current_camera(state.d, d->camera.get());
        auto stateset = model->getStateSet();
        if (stateset) {
            state.applyShader(stateset);
        }

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

void Renderer::setRenderOrder(RenderOrder order) {
    d->render_order = order;
}

Renderer::RenderOrder Renderer::getRenderOrder() const {
    return d->render_order;
}

void Renderer::setScene(Scene* scene) {
    d->scene = scene;
}

Scene* Renderer::getScene() const {
    return d->scene.get();
}

void Renderer::setUseMasterScene(bool val) {
    d->use_master_scene = val;
}

bool Renderer::getUseMasterScene() const {
    return d->use_master_scene;
}

void Renderer::setUseMasterViewport(bool val) {
    d->use_master_viewport = val;
}

bool Renderer::getUseMasterViewport() const {
    return d->use_master_viewport;
}

void Renderer::setUseMasterViewMatrix(bool val) {
    d->use_master_view_matrix = val;
}

bool Renderer::getUseMasterViewMatrix() const {
    return d->use_master_view_matrix;
}

void Renderer::setUseMasterProjectionMatrix(bool val) {
    d->use_master_proj_matrix = val;
}

bool Renderer::getUseMasterProjectionMatrix() const {
    return d->use_master_proj_matrix;
}

bool Renderer::handleEvent(Event* e) {
    auto handled = EventReceiver::handleEvent(e);

    if (false == handled) {
        if (d->cm.hasValue()) {
            handled |= d->cm->handleEvent(e);
        }
    }

    if(false == handled){
        if(d->scene.hasValue()){
            handled |= d->scene->handleEvent(e);
        }
    }

    return handled;
}

void Renderer::update(UpdateContext* ctx) {
    EventReceiver::update(ctx);
    if(d->scene.hasValue()){
        d->scene->update(ctx);
    }
}
} // namespace glr