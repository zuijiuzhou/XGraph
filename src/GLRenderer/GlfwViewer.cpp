#include "GlfwViewer.h"

#include <functional>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <vine/core/Ptr.h>

#include "Camera.h"
#include "CameraManipulator.h"
#include "Event.h"
#include "GraphicContext.h"
#include "Renderer.h"
#include "Viewer.h"

namespace glr {
namespace {
class GlfwGraphicContext : public GraphicContext {
  public:
    GlfwGraphicContext() {}
    virtual ~GlfwGraphicContext() {
        if (wnd_) {
            // d->ctx->releaseGLObjects();
            glfwDestroyWindow(wnd_);
        }
    }

  public:
    virtual void makeCurrent() override { glfwMakeContextCurrent(wnd_); }

    virtual void swapBuffers() override { glfwSwapBuffers(wnd_); }

    virtual void realize() {
        if (isRealized()) return;
        auto w = 800, h = 600;

        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        auto wnd = glfwCreateWindow(800, 600, "GlfwViewer", NULL, NULL);
        
        using GLFWFrameBufferSizeCallback = std::function<void(GLFWwindow*, int, int)>;
        using GLFWKeyCallback = std::function<void(GLFWwindow * window, int key, int scancode, int action, int mods)>;
        using GLFWMouseButtonCallback = std::function<void(GLFWwindow*, int, int, int)>;
        using GLFWCursorPosCallback   = std::function<void(GLFWwindow*, double, double)>;
        using GLFWScrollCallback      = std::function<void(GLFWwindow*, double, double)>;
        static GLFWFrameBufferSizeCallback framebuffer_size_callback;
        static GLFWKeyCallback             key_callback;
        static GLFWMouseButtonCallback     mouse_button_callback;
        static GLFWCursorPosCallback       cursor_pos_callback;
        static GLFWScrollCallback          scroll_callback;

        framebuffer_size_callback = [this](GLFWwindow* wnd, int w, int h) {
            this->framebuffer_size_callback(wnd, w, h);
        };
        key_callback = [this](GLFWwindow* wnd, int key, int scancode, int action, int mods) {
            this->key_callback(wnd, key, scancode, action, mods);
        };
        mouse_button_callback = [this](GLFWwindow* wnd, int button, int action, int mods) {
            this->mouse_button_callback(wnd, button, action, mods);
        };
        cursor_pos_callback = [this](GLFWwindow* wnd, double x, double y) {
            this->cursor_position_callback(wnd, x, y);
        };
        scroll_callback = [this](GLFWwindow* wnd, double x, double y) { this->scroll_callback(wnd, x, y); };

        glfwSetFramebufferSizeCallback(
            wnd,
            (GLFWframebuffersizefun)[](GLFWwindow * wnd, int w, int h) {
                static auto callback = framebuffer_size_callback;
                callback(wnd, w, h);
            });
        glfwSetKeyCallback(
            wnd,
            (GLFWkeyfun)[](GLFWwindow * wnd, int key, int scancode, int action, int mods) {
                static auto callback = key_callback;
                callback(wnd, key, scancode, action, mods);
            });
        glfwSetMouseButtonCallback(
            wnd,
            (GLFWmousebuttonfun)[](GLFWwindow * wnd, int button, int action, int mods) {
                auto callback = mouse_button_callback;
                callback(wnd, button, action, mods);
            });
        glfwSetCursorPosCallback(
            wnd,
            (GLFWcursorposfun)[](GLFWwindow * wnd, double x, double y) {
                auto callback = cursor_pos_callback;
                callback(wnd, x, y);
            });
        glfwSetScrollCallback(
            wnd,
            (GLFWscrollfun)[](GLFWwindow * wnd, double x, double y) {
                auto callback = scroll_callback;
                callback(wnd, x, y);
            });

        wnd_  = wnd;
        size_ = glm::vec2(w, h);
        glfwShowWindow(wnd_);
        GraphicContext::realize();
    }

    virtual int getWidth() const override { return size_.x; }

    virtual int getHeight() const override { return size_.y; }

    bool isWindowShouldClose() const { return glfwWindowShouldClose(wnd_); }

    void pollEvents() { glfwPollEvents(); }

  private:
    void error_callback(int error, const char* desc) {}

    void key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            return;
        }
        switch (key) {
        case GLFW_KEY_ESCAPE:
        {
            glfwSetWindowShouldClose(wnd, GL_TRUE);
        } break;

        default: break;
        }
    }

    void framebuffer_size_callback(GLFWwindow* wnd, int w, int h) {
        if (w == 0 || h == 0) return;
        notify(Event::createResizeEvent(this, w, h));
    }

    void mouse_button_callback(GLFWwindow* wnd, int button, int action, int mods) {
        auto btn = ButtonLeft;
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            btn = ButtonLeft;
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
            btn = ButtonMiddle;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            btn = ButtonRight;
        if (action == GLFW_PRESS) {
            notify(Event::createMousePressEvent(this, btn, cursor_pt_.x, cursor_pt_.y));
        }
        else if (action == GLFW_RELEASE) {
            notify(Event::createMouseReleaseEvent(this, btn, cursor_pt_.x, cursor_pt_.y));
        }
    }

    void cursor_position_callback(GLFWwindow* wnd, double x, double y) {
        cursor_pt_ = glm::vec2(x, y);
        notify(Event::createMouseMoveEvent(this, x, y));
    }

    void scroll_callback(GLFWwindow* wnd, double x, double y) { notify(Event::createMouseWheelEvent(this, y)); }

  private:
    GLFWwindow* wnd_;
    glm::vec2   size_;
    glm::vec2   cursor_pt_;
};
} // namespace

struct GlfwViewer::Data {
    vine::RefPtr<Viewer>             viewer;
    vine::RefPtr<Renderer>           renderer;
    vine::RefPtr<GlfwGraphicContext> ctx;
    bool                             is_initialized = false;
};

GlfwViewer::GlfwViewer()
  : d(new Data()) {
}

GlfwViewer::~GlfwViewer() {

    delete d;
}

void GlfwViewer::initialize() {
    if (d->is_initialized) return;

    auto viewer   = new Viewer();
    auto renderer = new Renderer();
    auto cam      = renderer->getCamera();
    auto cm       = new StandardCameraManipulator(cam);
    auto ctx      = new GlfwGraphicContext();

    renderer->setContext(ctx);
    renderer->setCameraManipulator(cm);

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

bool GlfwViewer::isInitialized() const {
    return d->is_initialized;
}

Viewer* GlfwViewer::getViewer() const {
    return d->viewer.get();
}

void GlfwViewer::run() {
    if (!isInitialized()) {
        initialize();
    }
    auto& ctx = *d->ctx.get();
    ctx.makeCurrent();
    while (!ctx.isWindowShouldClose()) {
        ctx.pollEvents();
        d->viewer->frame();
        ctx.swapBuffers();
    }
}

} // namespace glr