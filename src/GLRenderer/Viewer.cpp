#include "Viewer.h"
#include <map>
#include <vector>

#include <vine/core/Ptr.h>

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
        if (s > 1) {
            printf("\n%i", s);
        }
        while (auto e = events->pop()) {
            for (auto r : renderers) {
                auto cm = r->getCameraManipulator();
                if (cm) {
                    cm->handleEvent(e);
                }
            }
        }
    }
    RenderInfo info(this);
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