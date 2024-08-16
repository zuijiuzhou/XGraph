#include "Viewer.h"

#include <map>
#include <vector>

#include <vine/core/Ptr.h>

#include "Callbacks.h"
#include "Camera.h"
#include "CameraManipulator.h"
#include "GraphicContext.h"
#include "RenderInfo.h"
#include "Renderer.h"

namespace glr {
VI_OBJECT_META_IMPL(Viewer, Object);

struct Viewer::Data {
    std::vector<vine::RefPtr<Renderer>> renderers;
    vine::RefPtr<Renderer>              master_renderer;
};

Viewer::Viewer()
  : d(new Data()) {
}

Viewer::~Viewer() {
    delete d;
}

void Viewer::frame() {
    std::map<GraphicContext*, std::vector<Renderer*>> ctxs;
    for (auto& renderer : d->renderers) {
        auto  ctx = renderer->getContext();
        auto& rs  = ctxs[ctx];
        rs.push_back(renderer.get());
    }

    for (auto& kv : ctxs) {
        auto  ctx       = kv.first;
        auto& renderers = kv.second;
        auto  events    = ctx->getEventQueue();
        auto  s         = events->size();
        while (auto e = events->pop()) {
            for (auto r : renderers) {
                r->handleEvent(e);
            }
        }
    }

    struct UpdateContextImpl : public UpdateContext {

        virtual Renderer* getCurrentRenderer() const override { return current_renderer_; }
        virtual Renderer* getMasterRenderer() const override { return master_renderer_; }

        Renderer* current_renderer_ = nullptr;
        Renderer* master_renderer_  = nullptr;
    };

    UpdateContextImpl update_ctx;
    for (auto& kv : ctxs) {
        auto  ctx       = kv.first;
        auto& renderers = kv.second;
        auto  events    = ctx->getEventQueue();
        auto  s         = events->size();
        for (auto r : renderers) {
            update_ctx.current_renderer_ = r;
            r->update(&update_ctx);
        }
    }

    RenderInfo info(d->master_renderer.get());
    for (auto& renderer : d->renderers) {
        if (renderer->getRenderOrder() == Renderer::PRE_RENDER) {
            renderer->render(info);
        }
    }
    for (auto& renderer : d->renderers) {
        if (renderer->getRenderOrder() == Renderer::MID_RENDER) {
            renderer->render(info);
        }
    }
    for (auto& renderer : d->renderers) {
        if (renderer->getRenderOrder() == Renderer::POST_RENDER) {
            renderer->render(info);
        }
    }
}

int Viewer::run() {
    while (true) {
        frame();
    }
    return 0;
}

void Viewer::setMasterRenderer(Renderer* renderer) {
    if (d->master_renderer != renderer) {
        addRenderer(renderer);
        d->master_renderer = renderer;
    }
}

Renderer* Viewer::getMasterRenderer() const {
    return d->master_renderer.get();
}

void Viewer::addRenderer(Renderer* renderer) {
    if (!d->master_renderer) {
        d->master_renderer = renderer;
    }
    auto iter = std::find(d->renderers.begin(), d->renderers.end(), renderer);
    if (iter == d->renderers.end()) {
        d->renderers.push_back(renderer);
    }
}

int Viewer::getNbRenderers() const {
    return d->renderers.size();
}

Renderer* Viewer::getRendererAt(int idx) const {
    return d->renderers.at(idx).get();
}
} // namespace glr