#include "WglUtils.h"

typedef HGLRC WINAPI PROC_wglCreateContextAttribsARB(HDC hDC, HGLRC hShareContext, const int* attribList);
typedef BOOL WINAPI  PROC_wglSwapIntervalEXT(int interval);
typedef BOOL WINAPI  PROC_wglChoosePixelFormatARB(HDC          hDC,
                                                  const int*   piAttribIList,
                                                  const FLOAT* pfAttribFList,
                                                  UINT         nMaxFormats,
                                                  int*         piFormats,
                                                  UINT*        nNumFormats);
typedef BOOL WINAPI  PROC_wglGetPixelFormatAttribivARB(HDC, int, int, UINT, const int*, int*);

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_DRAW_TO_BITMAP_ARB 0x2002
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_STEREO_ARB 0x2012
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_COLOR_BITS_ARB 0x2014
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201a
#define WGL_ALPHA_BITS_ARB 0x201b
#define WGL_ALPHA_SHIFT_ARB 0x201c
#define WGL_ACCUM_BITS_ARB 0x201d
#define WGL_ACCUM_RED_BITS_ARB 0x201e
#define WGL_ACCUM_GREEN_BITS_ARB 0x201f
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_FULL_ACCELERATION_ARB 0x2027
#define WGL_TYPE_RGBA_ARB 0x202b
#define WGL_SAMPLE_BUFFERS_ARB 0x2041
#define WGL_SAMPLES_ARB 0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_RELEASE_BEHAVIOR_ARB 0x2097
#define WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB 0x2098
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20a9
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_ES2_PROFILE_BIT_EXT 0x00000004
#define WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004
#define WGL_COLORSPACE_SRGB_EXT 0x3089
#define WGL_COLORSPACE_EXT 0x309d
#define WGL_CONTEXT_OPENGL_NO_ERROR_ARB 0x31b3
#define WGL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define WGL_NO_RESET_NOTIFICATION_ARB 0x8261

namespace glutils {
static PROC_wglCreateContextAttribsARB*   wglCreateContextAttribsARB   = nullptr;
static PROC_wglSwapIntervalEXT*           wglSwapIntervalEXT           = nullptr;
static PROC_wglChoosePixelFormatARB*      wglChoosePixelFormatARB      = nullptr;
static PROC_wglGetPixelFormatAttribivARB* wglGetPixelFormatAttribivARB = nullptr;

static bool wgl_proc_init() {
    WNDCLASSA windowClass     = { 0 };
    windowClass.style         = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc   = DefWindowProcA;
    windowClass.hInstance     = GetModuleHandle(NULL);
    windowClass.lpszClassName = "glh_gl_loader_class";
    if (!RegisterClassA(&windowClass)) {
        return false;
    }

    HWND window = CreateWindowExA(0,
                                  windowClass.lpszClassName,
                                  "glh_gl_loader_window",
                                  0,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  CW_USEDEFAULT,
                                  0,
                                  0,
                                  GetModuleHandle(NULL),
                                  0);

    if (!window) {
        return false;
    }

    HDC                   windowDC = GetDC(window);
    PIXELFORMATDESCRIPTOR pfd      = { 0 };
    pfd.nSize                      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion                   = 1;
    pfd.iPixelType                 = PFD_TYPE_RGBA;
    pfd.dwFlags                    = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.cColorBits                 = 24;
    pfd.cAlphaBits                 = 8;
    pfd.cDepthBits                 = 24;
    pfd.cStencilBits               = 8;
    pfd.iLayerType                 = PFD_MAIN_PLANE;

    int                   pfi = ChoosePixelFormat(windowDC, &pfd);
    PIXELFORMATDESCRIPTOR spfd;
    DescribePixelFormat(windowDC, pfi, sizeof(spfd), &spfd);
    if (!SetPixelFormat(windowDC, pfi, &spfd)) {
        return false;
    }

    HGLRC glctx = wglCreateContext(windowDC);
    if (!wglMakeCurrent(windowDC, glctx)) {
        return false;
    }

    wglCreateContextAttribsARB = (PROC_wglCreateContextAttribsARB*)wglGetProcAddress("wglCreateContextAttribsARB");
    wglSwapIntervalEXT         = (PROC_wglSwapIntervalEXT*)wglGetProcAddress("wglSwapIntervalEXT");
    wglChoosePixelFormatARB    = (PROC_wglChoosePixelFormatARB*)wglGetProcAddress("wglChoosePixelFormatARB");
    wglGetPixelFormatAttribivARB =
        (PROC_wglGetPixelFormatAttribivARB*)wglGetProcAddress("wglGetPixelFormatAttribivARB");
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(glctx);
    ReleaseDC(window, windowDC);
    DestroyWindow(window);
    return true;
}

static HGLRC wgl_win32_create_context(HDC dc, int major, int minor, int core, int* pixelAttrs) {
    if (!wgl_proc_init()) {
        return nullptr;
    }

    int          pfi;
    unsigned int count;
    if (!wglChoosePixelFormatARB(dc, pixelAttrs, 0, 1, &pfi, &count)) {
        return NULL;
    }

    PIXELFORMATDESCRIPTOR spfd;
    DescribePixelFormat(dc, pfi, sizeof(spfd), &spfd);
    if (!SetPixelFormat(dc, pfi, &spfd)) {
        return NULL;
    }

    const int gl_attrs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB,
                             major,
                             WGL_CONTEXT_MINOR_VERSION_ARB,
                             minor,
                             WGL_CONTEXT_FLAGS_ARB,
                             0,
                             WGL_CONTEXT_PROFILE_MASK_ARB,
                             core ? 1 : 2,
                             0 };

    auto glctx = wglCreateContextAttribsARB(dc, 0, gl_attrs);
    if (!glctx) {
        return NULL;
    }

    if (!wglMakeCurrent(dc, glctx)) {
        return NULL;
    }

    wglSwapIntervalEXT(1);
    return glctx;
}

HGLRC WglContextManager::CreateByHwnd(HWND hWND, int major, int minor, int core) {
    HDC dc           = GetDC(hWND);
    int pixelAttrs[] = { WGL_DRAW_TO_WINDOW_ARB,
                         1,
                         WGL_ACCELERATION_ARB,
                         WGL_FULL_ACCELERATION_ARB,
                         WGL_SUPPORT_OPENGL_ARB,
                         1,
                         WGL_DOUBLE_BUFFER_ARB,
                         1,
                         WGL_PIXEL_TYPE_ARB,
                         WGL_TYPE_RGBA_ARB,
                         WGL_SAMPLE_BUFFERS_ARB,
                         1,
                         WGL_COLOR_BITS_ARB,
                         32,
                         WGL_DEPTH_BITS_ARB,
                         24,
                         WGL_STENCIL_BITS_ARB,
                         8,
                         0 };
    return wgl_win32_create_context(dc, major, minor, core, pixelAttrs);
}

HGLRC WglContextManager::CreateByBitmap(void** color_buffer, int width, int height, int major, int minor, int core) {
    // Create a memory DC compatible with the screen
    HDC hdc = CreateCompatibleDC(0);
    if (hdc == 0) return nullptr;

    // Create a bitmap compatible with the DC
    // must use CreateDIBSection(), and this means all pixel ops must be synchronised
    // using calls to GdiFlush() (see CreateDIBSection() docs)
    BITMAPINFO bmi = {
        { sizeof(BITMAPINFOHEADER), width, height, 1, 32, BI_RGB, 0, 0, 0, 0, 0 },
        { 0 }
    };
    HBITMAP hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)color_buffer, 0, 0);
    if (hbm == 0) return nullptr;

    // HDC hdcScreen = GetDC(0);
    // HBITMAP hbm = CreateCompatibleBitmap(hdcScreen,WIDTH,HEIGHT);

    // Select the bitmap into the DC
    HGDIOBJ r = SelectObject(hdc, hbm);
    if (r == 0) return nullptr;

    // Choose the pixel format
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),           // struct size
        1,                                       // Version number
        PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL, // use OpenGL drawing to BM
        PFD_TYPE_RGBA,                           // RGBA pixel values
        32,                                      // color bits
        0,
        0,
        0, // RGB bits shift sizes...
        0,
        0,
        0, // Don't care about them
        0,
        0, // No alpha buffer info
        0,
        0,
        0,
        0,
        0,              // No accumulation buffer
        32,             // depth buffer bits
        0,              // No stencil buffer
        0,              // No auxiliary buffers
        PFD_MAIN_PLANE, // Layer type
        0,              // Reserved (must be 0)
        0,              // No layer mask
        0,              // No visible mask
        0,              // No damage mask
    };
    int pfid = ChoosePixelFormat(hdc, &pfd);
    if (pfid == 0) return nullptr;

    // Set the pixel format
    // - must be done *after* the bitmap is selected into DC
    if (!SetPixelFormat(hdc, pfid, &pfd)) return nullptr;

    // Create the OpenGL resource context (RC) and make it current to the thread
    HGLRC glrc = wglCreateContext(hdc);
    if (glrc == 0) return nullptr;
    wglMakeCurrent(hdc, glrc);

    return glrc;
    // int pixelAttrs[] = {
    //     WGL_DRAW_TO_BITMAP_ARB, 1,
    //     // WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
    //     WGL_SUPPORT_OPENGL_ARB, 1,
    //     // WGL_DOUBLE_BUFFER_ARB, 1,
    //     WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
    //     WGL_SAMPLE_BUFFERS_ARB, 1,
    //     WGL_COLOR_BITS_ARB, 32,
    //     WGL_DEPTH_BITS_ARB, 24,
    //     WGL_STENCIL_BITS_ARB, 8,
    //     0};
    // return wgl_win32_create_context(dc, major, minor, core, pixelAttrs);
}

} // namespace glutils
