#include "GLVersion.h"

#include <mutex>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace glr {

static void queryVersionInfo() {
}

bool queryCurrentGLVersion(GLVersionInfo& vi) {
    return false;
}

bool queryDefaultGLVersion(GLVersionInfo& vi) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    auto wnd = glfwCreateWindow(1, 1, "queryGLVersion", NULL, NULL);
    if (!wnd) {
    }
    glfwMakeContextCurrent(wnd);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        glfwDestroyWindow(wnd);
        glfwTerminate();
        return false;
    }

    auto glv = GLVersionInfo();

    vi = glv;
    return true;
}
} // namespace glr
