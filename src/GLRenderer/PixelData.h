#pragma once

#include "Buffer.h"

namespace glr {
class PixelData : public Buffer {
    VI_OBJECT_META;

  public:
    /*
      glReadPixels
      glTexImage2D
      glTexSubImage2D
    */
    enum ExternalFormat
    {
        EF_RED             = GL_RED,
        EF_GREEN           = GL_GREEN,
        EF_BLUE            = GL_BLUE,
        EF_RG              = GL_RG,
        EF_RGB             = GL_RGB,
        EF_BGR             = GL_BGR,
        EF_RGBA            = GL_RGBA,
        EF_BGRA            = GL_BGRA,
        EF_DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        EF_STENCIL_INDEX   = GL_STENCIL_INDEX,
        EF_DEPTH_STENCIL   = GL_DEPTH_STENCIL,

        EF_RED_INTEGER   = GL_RED_INTEGER,
        EF_GREEN_INTEGER = GL_GREEN_INTEGER,
        EF_BLUE_INTEGER  = GL_BLUE_INTEGER,
        EF_RG_INTEGER    = GL_RG_INTEGER,
        EF_RGB_INTEGER   = GL_RGB_INTEGER,
        EF_BGR_INTEGER   = GL_BGR_INTEGER,
        EF_RGBA_INTEGER  = GL_RGBA_INTEGER,
        EF_BGRA_INTEGER  = GL_BGRA_INTEGER,

        // GL2
        EF_LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA,
        EF_LUMINANCE       = GL_LUMINANCE
    };

    enum InternalFormat
    {
        // BASE
        IF_DEPTH_COMPONENT  = GL_DEPTH_COMPONENT,
        IF_GL_DEPTH_STENCIL = GL_DEPTH_STENCIL,
        IF_STENCIL_INDEX    = GL_STENCIL_INDEX,
        IF_RED              = GL_RED,
        IF_RG               = GL_RG,
        IF_RGB              = GL_RGB,
        IF_RGBA             = GL_RGBA,

        // 1-CHANNEL
        IF_R8    = GL_R8,
        IF_R8I   = GL_R8I,
        IF_R8UI  = GL_R8UI,
        IF_R16   = GL_R16,
        IF_R16F  = GL_R16F,
        IF_R16I  = GL_R16I,
        IF_R16UI = GL_R16UI,

        // 2-CHANNELS
        IF_RG8   = GL_RG8,
        IF_RG16  = GL_RG16,
        IF_RG16F = GL_RG16F,
        IF_RGI   = GL_RG16I,
        IF_RGUI  = GL_RG16UI,

        // 3-CHANNELS
        // RGB8
        IF_RGB8    = GL_RGB8,
        IF_RGB8I   = GL_RGB8I,
        IF_RGB8UI  = GL_RGB8UI,
        // RGB10
        IF_RGB10   = GL_RGB10,
        // RGB12
        IF_RGB12   = GL_RGB12,
        // RGB16
        IF_RGB16   = GL_RGB16,
        IF_RGB16I  = GL_RGB16I,
        IF_RGB16UI = GL_RGB16UI,
        IF_RGB16F  = GL_RGB16F,
        // RGB32
        IF_RGB32I  = GL_RGB32I,
        IF_RGB32UI = GL_RGB32UI,
        IF_RGB32F  = GL_RGB32F,

        // 4-CHANNELS
        // RGBA8
        IF_RGBA8    = GL_RGBA8,
        IF_RGBA8I   = GL_RGBA8I,
        IF_RGBA8UI  = GL_RGBA8UI,
        // RGBA12
        IF_RGBA12   = GL_RGBA12,
        // RGBA16
        IF_RGBA16   = GL_RGBA16,
        IF_RGBA16F  = GL_RGBA16F,
        IF_RGBA16I  = GL_RGBA16I,
        IF_RGBA16UI = GL_RGBA16UI,
        // RGBA32
        IF_RGBA32I  = GL_RGBA32I,
        IF_RGBA32UI = GL_RGBA32UI,
        IF_RGBA32F  = GL_RGBA32F,

        // SRGB
        IF_SRGB  = GL_SRGB,
        IF_SRGB8 = GL_SRGB8,

        // DEPTH
        IF_DEPTH_COMPONENT16  = GL_DEPTH_COMPONENT16,
        IF_DEPTH_COMPONENT24  = GL_DEPTH_COMPONENT24,
        IF_DEPTH_COMPONENT32  = GL_DEPTH_COMPONENT32,
        IF_DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,

        // STENCIL
        IF_STENCIL_INDEX1  = GL_STENCIL_INDEX1,
        IF_STENCIL_INDEX4  = GL_STENCIL_INDEX4,
        IF_STENCIL_INDEX8  = GL_STENCIL_INDEX8,
        IF_STENCIL_INDEX16 = GL_STENCIL_INDEX16,

        // DEPTH_STENCIL
        IF_DEPTH24_STENCIL8  = GL_DEPTH24_STENCIL8,
        IF_DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8
    };

    enum DataType
    {
        DT_BYTE                           = GL_BYTE,
        DT_UNSIGNED_SHORT                 = GL_UNSIGNED_SHORT,
        DT_SHORT                          = GL_SHORT,
        DT_UNSIGNED_INT                   = GL_UNSIGNED_INT,
        DT_INT                            = GL_INT,
        DT_HALF_FLOAT                     = GL_HALF_FLOAT,
        DT_FLOAT                          = GL_FLOAT,
        DT_UNSIGNED_BYTE_3_3_2            = GL_UNSIGNED_BYTE_3_3_2,
        DT_UNSIGNED_BYTE_2_3_3_REV        = GL_UNSIGNED_BYTE_2_3_3_REV,
        DT_UNSIGNED_SHORT_5_6_5           = GL_UNSIGNED_SHORT_5_6_5,
        DT_UNSIGNED_SHORT_5_6_5_REV       = GL_UNSIGNED_SHORT_5_6_5_REV,
        DT_UNSIGNED_SHORT_4_4_4_4         = GL_UNSIGNED_SHORT_4_4_4_4,
        DT_UNSIGNED_SHORT_4_4_4_4_REV     = GL_UNSIGNED_SHORT_4_4_4_4_REV,
        DT_UNSIGNED_SHORT_5_5_5_1         = GL_UNSIGNED_SHORT_5_5_5_1,
        DT_UNSIGNED_SHORT_1_5_5_5_REV     = GL_UNSIGNED_SHORT_1_5_5_5_REV,
        DT_UNSIGNED_INT_8_8_8_8           = GL_UNSIGNED_INT_8_8_8_8,
        DT_UNSIGNED_INT_8_8_8_8_REV       = GL_UNSIGNED_INT_8_8_8_8_REV,
        DT_UNSIGNED_INT_10_10_10_2        = GL_UNSIGNED_INT_10_10_10_2,
        DT_UNSIGNED_INT_2_10_10_10_REV    = GL_UNSIGNED_INT_2_10_10_10_REV,
        DT_UNSIGNED_INT_24_8              = GL_UNSIGNED_INT_24_8,
        DT_UNSIGNED_INT_10F_11F_11F_REV   = GL_UNSIGNED_INT_10F_11F_11F_REV,
        DT_UNSIGNED_INT_5_9_9_9_REV       = GL_UNSIGNED_INT_5_9_9_9_REV,
        DT_FLOAT_32_UNSIGNED_INT_24_8_REV = GL_FLOAT_32_UNSIGNED_INT_24_8_REV
    };

  public:
    PixelData();

  private:
    VI_OBJECT_DATA;
};
} // namespace glr