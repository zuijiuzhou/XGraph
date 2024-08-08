#include "Image.h"

#include <glad/glad.h>
#include <vine/core/Exception.h>

namespace glr {
namespace {
inline int getNbChannelsOfFormat(Image::Format format) {
    switch (format) {
    case Image::R8:
    case Image::G8:
    case Image::B8: return 1;
    case Image::RGB888: return 3;
    case Image::RGBA8888: return 4;
    default: return 0;
    }
}

inline int getPixelSizeOfFormat(Image::Format format) {
    switch (format) {
    case Image::R8:
    case Image::G8:
    case Image::B8: return 1;
    case Image::RGB888: return 3;
    case Image::RGBA8888: return 4;
    default: return 0;
    }
}
} // namespace

VI_OBJECT_META_IMPL(Image, Object);

struct Image::Data {
    unsigned char* data   = nullptr;
    Format         format = Format::EMPTY;
    int            w      = 0;
    int            h      = 0;
};

Image::Image()
  : d(new Data()) {
}

void Image::setImage(int w, int h, Format format, const unsigned char* data) {
    auto& id = d->data;
    if (id) {
        free(id);
        id        = nullptr;
        d->w      = 0;
        d->h      = 0;
        d->format = EMPTY;
    }

    if (w <= 0 || h <= 0 || format == EMPTY) return;

    d->w      = w;
    d->h      = h;
    d->format = format;

    auto size_of_pixel = getPixelSizeOfFormat(format);
    auto size_of_img   = static_cast<size_t>(w) * h * size_of_pixel;
    id                 = static_cast<unsigned char*>(malloc(size_of_img));
    memcpy(id, data, size_of_img);
}

void Image::setImage(int w, int h, Format format, const unsigned char* data, int stride, int offset) {
    auto& id = d->data;
    if (id) {
        free(id);
        id        = nullptr;
        d->w      = 0;
        d->h      = 0;
        d->format = EMPTY;
    }

    if (w <= 0 || h <= 0 || format == EMPTY) return;

    if ((format == RGB888 && stride < 3) || (format == RGBA8888 && stride < 4) || offset >= stride)
        throw vine::Exception(vine::Exception::INVALID_ARGUMENTS);

    d->w      = w;
    d->h      = h;
    d->format = format;

    auto size_of_pixel = getPixelSizeOfFormat(format);
    auto size_of_img   = static_cast<size_t>(w) * h * size_of_pixel;
    id                 = static_cast<unsigned char*>(malloc(size_of_img));

    auto pix = data;
    for (int r = 0; r < h; r++) {
        for (int c = 0; c < w; w++) {
            memccpy(id, pix + offset, 0, size_of_pixel);
            id += size_of_pixel;
            pix += stride;
        }
    }
}

Image::Format Image::getFormat() const {
    return d->format;
}

int Image::getGLFormat() const {
    switch (d->format) {
    case R8: return GL_RED;
    case G8: return GL_GREEN;
    case B8: return GL_BLUE;
    case RGB888: return GL_RGB;
    case RGBA8888: return GL_RGBA;
    default: return GL_ZERO;
    }
}

int Image::getWidth() const {
    return d->w;
}

int Image::getHeight() const {
    return d->h;
}

int Image::getChannels() const {
    return getNbChannelsOfFormat(d->format);
}

unsigned char* Image::data() const {
    return d->data;
}

bool Image::isNull() const {
    return d->data == nullptr;
}
} // namespace glr