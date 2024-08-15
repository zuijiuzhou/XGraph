#include "RttRenderer.h"

#include <iostream>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <vine/core/Ptr.h>

#include "FrameBufferObject.h"
#include "Texture2D.h"

namespace glr {
VI_OBJECT_META_IMPL(RttRenderer, Renderer);

struct RttRenderer::Data {
    GLFWwindow*             wnd  = nullptr;
    GLuint                  fbo_ = 0;
    vine::RefPtr<Texture2D> color_buffer_;
    GLsizei                 w = 640, h = 360;
    bool                    is_initialized = false;
};

RttRenderer::RttRenderer()
  : d(new Data()) {
}

RttRenderer::~RttRenderer() {
}

void RttRenderer::resize(int w, int h) {
    if (w < 0 || h < 0) throw std::exception();
    d->w = w;
    d->h = h;
}

GLsizei RttRenderer::getWidth() const {
    return d->w;
}

GLsizei RttRenderer::getHeight() const {
    return d->h;
}

void RttRenderer::initialize() {
    if (!glfwInit()) {
        throw std::exception("GLFW init failed");
    }

    auto w = 1, h = 1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    auto wnd = glfwCreateWindow(800, 600, "RttRenderer", NULL, NULL);
    glfwMakeContextCurrent(wnd);
    if (!glad_glClear && !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(wnd);
        glfwTerminate();
        std::cout << "Failed to initialize GLAD" << std::endl;
        throw std::exception("GLAD init failed");
    }
    d->wnd            = wnd;
    d->is_initialized = true;
}

bool RttRenderer::isInitialized() const {
    return d->is_initialized;
}

Texture* RttRenderer::getColorBuffer() const {
    return d->color_buffer_.get();
}

} // namespace glr