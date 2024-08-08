#pragma once
#include <string>

#include "Object.h"

namespace glr {
class Image : public Object {
    VI_OBJECT_META;
    VI_DISABLE_COPY_MOVE(Image);

  public:
    enum Format
    {
        EMPTY,
        R8,
        G8,
        B8,
        RGB888,
        RGBA8888,
    };

  public:
    Image();

  public:
    void setImage(int w, int h, Format format, const unsigned char* data);

    void setImage(int w, int h, Format format, const unsigned char* data, int stride, int offset);

    Format getFormat() const;

    int getGLFormat() const;

    int getWidth() const;

    int getHeight() const;

    int getChannels() const;

    unsigned char* data() const;

    bool isNull() const;

  private:
    struct Data;
    Data* const d;
};
} // namespace glr