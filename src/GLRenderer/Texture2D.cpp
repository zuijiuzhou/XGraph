#include "Texture2D.h"

#include <vine/core/Ptr.h>

#include "Image.h"
#include "ImageLoader.h"

namespace glr {
VI_OBJECT_META_IMPL(Texture2D, Texture);

struct Texture2D::Data {
    GLsizei             w         = 0;
    GLsizei             h         = 0;
    GLint               in_format = GL_RGB;
    vine::RefPtr<Image> img       = nullptr;
};

Texture2D::Texture2D()
  : d(new Data()) {
}

Texture2D::~Texture2D() {
    delete d;
}

Texture::Type Texture2D::getType() const {
    return Type::TEXTURE_2D;
}

void Texture2D::setWidth(GLsizei w) {
    d->w = w;
    dirty();
}

void Texture2D::setHeight(GLsizei h) {
    d->h = h;
    dirty();
}

GLsizei Texture2D::getWidth() const {
    return d->w;
}

GLsizei Texture2D::getHeight() const {
    return d->h;
}

void Texture2D::setImage(const std::string& img_file) {
    auto img = ImageLoader().loadFile(img_file);
    setImage(img);
}

void Texture2D::setImage(Image* image) {
    if (image == d->img) return;

    d->img = image;
    d->w   = 0;
    d->h   = 0;
    if (image) {
        d->w = image->getWidth();
        d->h = image->getHeight();
    }
    dirty();
}

bool Texture2D::save(const std::string& path) const {
    return false;
}

GLuint Texture2D::onCreate(State& ctx) {
    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(getType(), id);
    glTexParameteri(getType(), GL_TEXTURE_MIN_FILTER, getFilter(MIN_FILTER));
    glTexParameteri(getType(), GL_TEXTURE_MAG_FILTER, getFilter(MAX_FILTER));
    glTexParameteri(getType(), GL_TEXTURE_WRAP_S, getWrap(WRAP_S));
    glTexParameteri(getType(), GL_TEXTURE_WRAP_T, getWrap(WRAP_T));

    void* img_data = nullptr;
    GLint w, h, fmt, in_format;

    if (d->img.get()) {
        img_data  = d->img->data();
        w         = d->img->getWidth();
        h         = d->img->getHeight();
        fmt       = d->img->getGLFormat();
        in_format = fmt;
    }
    else {
        w         = d->w;
        h         = d->h;
        fmt       = GL_RGB;
        in_format = d->in_format;
    }

    if (w && h) {
        glTexImage2D(GL_TEXTURE_2D, 0, in_format, w, h, 0, fmt, GL_UNSIGNED_BYTE, img_data);
    }

    return id;
}

bool Texture2D::onUpdate(State& ctx) {
    return true;
}

} // namespace glr