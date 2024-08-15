#pragma once

#include "glrenderer_global.h"

namespace glr {
struct GLRENDERER_API AppInitializationParameters {
    int  gl_ver_maj          = 4;
    int  gl_ver_min          = 6;
    bool gl_use_core_profile = true;
};

class GLRENDERER_API AppInitializer {
  public:
    AppInitializer(const AppInitializationParameters& params);

  public:
    virtual void initGlfw();

    virtual void initGlad();

    virtual void initQt();

    virtual bool isGlfwInitialized() const;

    virtual bool isGladInitialized() const;

    virtual bool isQtInitialized() const;

  private:
    AppInitializationParameters params_;
};
} // namespace glr