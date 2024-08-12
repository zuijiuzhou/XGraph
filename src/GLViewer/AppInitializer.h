#pragma once

namespace glv {
struct AppInitializationParameters {
    int  gl_ver_maj      = 3;
    int  gl_ver_min      = 3;
    bool gl_use_core_profile = true;
};

class AppInitializer {
  public:
    AppInitializer(const AppInitializationParameters& params);

  public:
    virtual void initGlfw();

    virtual void initGlad();

    virtual void initOpenSceneGraph();

    virtual bool isGlfwInitialized() const;

    virtual bool isGladInitialized() const;

    virtual bool isOpenSceneGraphInitialized() const;
    
  private:
    AppInitializationParameters params_;
};
} // namespace glv