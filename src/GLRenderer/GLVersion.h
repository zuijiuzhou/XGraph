#pragma once

namespace glr {
struct GLVersionInfo {
    int major;
    int minor;

    bool glslSupported;
    bool fboSupported;
    bool drawBuffersSupported;

    bool npotSupported;
};

bool queryCurrentGLVersion(GLVersionInfo& vi);

bool queryDefaultGLVersion(GLVersionInfo& vi);
} // namespace glr