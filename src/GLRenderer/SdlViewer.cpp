#include "SdlViewer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include <vine/core/Ptr.h>

#include "Camera.h"
#include "CameraManipulator.h"
#include "Event.h"
#include "GraphicContext.h"
#include "Renderer.h"
#include "Viewer.h"

namespace glr {
namespace {
class SdlGraphicContext : public GraphicContext {
  public:
    SdlGraphicContext() {}

    virtual ~SdlGraphicContext() {
        if (sdl_wnd_) {
            SDL_GL_DeleteContext(sdl_ctx_);
            SDL_DestroyWindow(sdl_wnd_);
        }
    }

  public:
    virtual void makeCurrent() override { SDL_GL_MakeCurrent(sdl_wnd_, sdl_ctx_); }

    virtual void swapBuffers() override { SDL_GL_SwapWindow(sdl_wnd_); }

    virtual void realize() override {
        if (isRealized()) return;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::exception("SDL init failed");
        }

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        auto w = 800, h = 600;

        auto* sdl_wnd =
            SDL_CreateWindow("SdlViewer", 0, 0, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
        if (sdl_wnd == NULL) {
            throw std::exception("Unable to create SDL window");
        }

        SDL_SysWMinfo sdlInfo;
        SDL_VERSION(&sdlInfo.version);
        SDL_GetWindowWMInfo(sdl_wnd, &sdlInfo);

        auto sdl_ctx = SDL_GL_CreateContext(sdl_wnd);
        if (sdl_ctx == NULL) {
            SDL_DestroyWindow(sdl_wnd);
            SDL_Quit();
            throw std::exception("Unable to create SDL context");
        }

        SDL_GL_SetSwapInterval(0);
        SDL_GL_MakeCurrent(sdl_wnd, sdl_ctx);

        if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
            SDL_GL_DeleteContext(sdl_ctx);
            SDL_DestroyWindow(sdl_wnd);
            SDL_Quit();
            throw std::exception("GLAD init failed");
        }

        SDL_ShowWindow(sdl_wnd);
        SDL_GL_MakeCurrent(sdl_wnd_, sdl_ctx);
        sdl_wnd_ = sdl_wnd;
        sdl_ctx_ = sdl_ctx;
        GraphicContext::realize();
    }

    virtual int getWidth() const override { return size_.x; }

    virtual int getHeight() const override { return size_.y; }

    void pollEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) done_ = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(sdl_wnd_))
                done_ = true;

            if (done_) break;
        }
    }

    bool isDone() { return done_; }

  private:
    SDL_Window*   sdl_wnd_;
    SDL_GLContext sdl_ctx_;
    glm::vec2     size_;
    bool          done_ = false;
};
} // namespace

struct SdlViewer::Data {
    vine::RefPtr<Viewer>            viewer;
    vine::RefPtr<Renderer>          renderer;
    vine::RefPtr<SdlGraphicContext> ctx;
    bool                            is_initialized = false;
};

SdlViewer::SdlViewer()
  : d(new Data()) {
}

SdlViewer::~SdlViewer() {
    delete d;
}

void SdlViewer::initialize() {
    if (d->is_initialized) return;

    auto viewer   = new Viewer();
    auto renderer = new Renderer();
    auto cam      = renderer->getCamera();
    auto cm       = new StandardCameraManipulator(cam);
    auto ctx      = new SdlGraphicContext();
    renderer->setContext(ctx);

    cam->setViewport(0., 0., ctx->getWidth(), ctx->getHeight());
    cam->setClearDepth(1.0);
    cam->setClearStencil(1);
    cam->setClearColor(glm::vec4(0., 0., 0., 1.));
    cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    viewer->addRenderer(renderer);
    ctx->realize();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);

    d->viewer         = viewer;
    d->renderer       = renderer;
    d->ctx            = ctx;
    d->is_initialized = true;
}

bool SdlViewer::isInitialized() const {
    return d->is_initialized;
}

Viewer* SdlViewer::getViewer() const {
    return d->viewer.get();
}

void SdlViewer::run() {
    if (!isInitialized()) {
        initialize();
    }
    auto& ctx = *d->ctx.get();
    ctx.makeCurrent();
    while (!ctx.isDone()) {
        ctx.pollEvents();
        d->viewer->frame();
        ctx.swapBuffers();
    }
}
} // namespace glr