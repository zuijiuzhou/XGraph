#include "CubeMap.h"

#include <type_traits>

#include "Image.h"
#include "ImageLoader.h"

namespace glr {
VI_OBJECT_META_IMPL(CubeMap, Texture);

struct CubeMap::Data {
    std::vector<vine::RefPtr<Image>> imgs;
    GLint                            in_format = GL_RGB;
};

CubeMap::CubeMap()
  : d(new Data()) {
}

CubeMap::~CubeMap() {
}

Texture::Type CubeMap::getType() const {
    return Type::TEXTURE_CUBE_MAP;
}

void CubeMap::setImages(const std::vector<std::string>& imgs) {
    ImageLoader         il;
    std::vector<Image*> is;
    is.reserve(imgs.size());
    for (auto img : imgs) {
        is.push_back(il.loadFile(img));
    }
    setImages(is);
}

void CubeMap::setImages(const std::vector<Image*>& imgs) {

    std::vector<vine::RefPtr<Image>> is;
    is.reserve(imgs.size());
    for (auto img : imgs) is.push_back(img);
    d->imgs = std::move(is);
    dirty();
}

GLuint CubeMap::onCreate(State& state) {
    if (d->imgs.size() != 6) return 0;

    GLuint id = 0;
    glGenTextures(1, &id);
    glBindTexture(getType(), id);
    glTexParameteri(getType(), GL_TEXTURE_MIN_FILTER, getFilter(MIN_FILTER));
    glTexParameteri(getType(), GL_TEXTURE_MAG_FILTER, getFilter(MAX_FILTER));
    glTexParameteri(getType(), GL_TEXTURE_WRAP_S, getWrap(WRAP_S));
    glTexParameteri(getType(), GL_TEXTURE_WRAP_T, getWrap(WRAP_T));
    glTexParameteri(getType(), GL_TEXTURE_WRAP_R, getWrap(WRAP_R));

    for (int i = 0; i < 6; i++) {
        auto& img = d->imgs[i];

        if (img->isNull()) continue;

        auto w        = img->getWidth();
        auto h        = img->getHeight();
        auto fmt      = img->getGLFormat();
        auto in_fmt   = d->in_format;
        auto img_data = img->data();

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, in_fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, img_data);
    }

    return id;
}

bool CubeMap::onUpdate(State& state) {
    return true;
}

} // namespace glr