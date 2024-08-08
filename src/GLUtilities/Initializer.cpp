#include "Initializer.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace glutils {

namespace {

bool s_is_glfw_initialized = false;
bool s_is_glad_initialized = false;

void initGlfw() {
}
void initGlad() {
    if (glfwInit() == GLFW_FALSE) {
        throw std::exception("Failed to initialize GLFW.");
    }
    printf("Initialization of GLFW succeeded\n");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}
} // namespace

Initializer::Initializer() {
}

void Initializer::initGlfw() {
    if (isGlfwInitialized()){
         return;
    }

    if (glfwInit() == GLFW_FALSE){
        throw std::exception("Failed to initialize GLFW.");
    }

    printf("Initialization of GLFW succeeded\n");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    s_is_glfw_initialized = true;
}

void Initializer::initGlad() {
    initGlfw();
    auto wnd = glfwCreateWindow(1, 1, "GladInitializer", NULL, NULL);

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

bool Initializer::isGlfwInitialized() const {
    return s_is_glfw_initialized;
}

bool Initializer::isGladInitialized() const {
    return s_is_glad_initialized;
}

} // namespace glutils