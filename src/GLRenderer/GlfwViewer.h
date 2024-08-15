#pragma once

#include "glrenderer_global.h"

namespace glr {
class Viewer;

class GLRENDERER_API GlfwViewer {
  public:
    GlfwViewer();
    virtual ~GlfwViewer();

  public:
    Viewer* getViewer() const;

    void initialize();

    bool isInitialized() const;

    void run();

  private:
    struct Data;
    Data* const d;
};
}; // namespace glr