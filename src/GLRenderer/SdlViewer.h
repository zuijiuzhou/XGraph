#pragma once
#include <glad/glad.h>

namespace glr {
class Viewer;

class SdlViewer {
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