#pragma once
#include "glrenderer_global.h"

#include <Windows.h>

namespace glutils {
class GLRENDERER_API WglContextManager {
  public:
    static HGLRC CreateByHwnd(HWND hWnd, int major, int minor, int core);

    static HGLRC CreateByBitmap(void** color_buffer, int width, int height, int major, int minor, int core);
};
} // namespace glutils
