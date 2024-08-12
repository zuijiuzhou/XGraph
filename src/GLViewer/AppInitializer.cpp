#include "AppInitializer.h"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <osgDB/Registry>
#include <osg/Notify>

#include "StdUtils/Environment.h"

namespace glv {
namespace {

bool s_is_glfw_initialized = false;
bool s_is_glad_initialized = false;
bool s_is_osg_initialized  = false;

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

void AppInitializer::initOpenSceneGraph() {
    if (isOpenSceneGraphInitialized()) {
        return;
    }
    osg::setNotifyLevel(osg::DEBUG_FP);
    auto paths = osgDB::Registry::instance()->getLibraryFilePathList();
    auto new_plugin_dir = stdutils::getApplicationDir();
    new_plugin_dir += "\\plugins\\osg";
    paths.insert(paths.begin(), new_plugin_dir);
    osgDB::Registry::instance()->setLibraryFilePathList(paths);
}

bool AppInitializer::isGlfwInitialized() const {
    return s_is_glfw_initialized;
}

bool AppInitializer::isGladInitialized() const {
    return s_is_glad_initialized;
}
bool AppInitializer::isOpenSceneGraphInitialized() const {
    return s_is_osg_initialized;
}
} // namespace glv