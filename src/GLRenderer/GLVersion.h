#pragma once

#include "glrenderer_global.h"

namespace glr {
struct GLRENDERER_API GLVersionInfo {
    int major;
    int minor;

    bool glslSupported;
    bool fboSupported;
    bool drawBuffersSupported;

    bool npotSupported;
};

GLRENDERER_API bool queryCurrentGLVersion(GLVersionInfo& vi);

GLRENDERER_API bool queryDefaultGLVersion(GLVersionInfo& vi);
} // namespace glr