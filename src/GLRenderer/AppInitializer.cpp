#include "AppInitializer.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <QSurfaceFormat>
#include <QApplication>

#include "StdUtils/Environment.h"

namespace glr {
namespace {

bool s_is_glfw_initialized = false;
bool s_is_glad_initialized = false;
bool s_is_qt_initialized = false;

} // namespace

AppInitializer::AppInitializer(const AppInitializationParameters& params)
  : params_(params) {
}

void AppInitializer::initGlfw() {
    if (isGlfwInitialized()) {
        return;
    }

    if (glfwInit() == GLFW_FALSE) {
        throw std::exception("Failed to initialize GLFW.");
    }

    printf("Initialization of GLFW succeeded\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params_.gl_ver_maj);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params_.gl_ver_min);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    if (params_.gl_use_core_profile) {
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }
    else {

        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    }

    s_is_glfw_initialized = true;
}

void AppInitializer::initGlad() {
    if (isGladInitialized()) {
        return;
    }
    initGlfw();
    auto wnd = glfwCreateWindow(1, 1, "GladAppInitializer", NULL, NULL);

    if (!wnd) {
        throw std::exception("Failed to create window using glfw.");
    }

    glfwMakeContextCurrent(wnd);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(wnd);
        throw std::exception("Failed to initialize GLAD.");
    }

    glfwDestroyWindow(wnd);
    printf("Initialization of GLAD succeeded\n");
}

void AppInitializer::initQt() {
    if(isQtInitialized()){
        return;
    }
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(4);
    format.setVersion(params_.gl_ver_maj, params_.gl_ver_min);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setColorSpace(QSurfaceFormat::ColorSpace::DefaultColorSpace);
    format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);

    QSurfaceFormat::setDefaultFormat(format);
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts, false);
}

bool AppInitializer::isGlfwInitialized() const {
    return s_is_glfw_initialized;
}

bool AppInitializer::isGladInitialized() const {
    return s_is_glad_initialized;
}
bool AppInitializer::isQtInitialized() const {
    return s_is_qt_initialized;
}
} // namespace glr