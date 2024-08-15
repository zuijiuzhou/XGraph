#pragma once

#include "glrenderer_global.h"

namespace glr {
class Viewer;

class GLRENDERER_API SdlViewer {
  public:
    SdlViewer();
    virtual ~SdlViewer();

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