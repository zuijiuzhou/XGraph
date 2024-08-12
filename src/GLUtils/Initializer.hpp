#pragma once

namespace glutils {

struct InitializationParameters{
    int gl_ver_maj = 3;
    int gl_ver_min = 3;
    bool gl_core_profile = true;
};

class Initializer {
  public:
    Initializer();

  public:
    virtual void initGlfw();

    virtual void initGlad();

    virtual bool isGlfwInitialized() const;

    virtual bool isGladInitialized() const;
};
} // namespace glutils