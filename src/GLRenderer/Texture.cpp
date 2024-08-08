
#include "Texture.h"
#include "stdrenderer_global.h"

namespace glr {
VI_OBJECT_META_IMPL(Texture, PixelData);

struct Texture::Data {
    WrapMode wrap_s = CLAMP_TO_EDGE;
    WrapMode wrap_t = CLAMP_TO_EDGE;
    WrapMode wrap_r = CLAMP_TO_EDGE;

    FilterMode filter_min = LINEAR;
    FilterMode filter_max = LINEAR;
};

Texture::Texture()
  : d(new Data()) {
}

Texture::~Texture() {
    delete d;
}

void Texture::onBind(State& state) {
    auto type = getType();
    glBindTexture(type, getId(state));
}

void Texture::onUnbind(State& state) {
    auto type = getType();
    glBindTexture(type, 0);
}

void Texture::onRelease(State& state) {
    auto id = getId(state);
    glDeleteTextures(1, &id);
}

void Texture::setFilter(FilterParameter param, FilterMode mode) {
    if (param == MAX_FILTER) {
        if (mode == d->filter_max) return;
        d->filter_max = mode;
        dirty();
    }
    else if (param == MIN_FILTER) {
        if (mode == d->filter_min) return;
        d->filter_min = mode;
        dirty();
    }
}

Texture::FilterMode Texture::getFilter(FilterParameter param) const {
    if (param == MAX_FILTER)
        return d->filter_max;
    else
        return d->filter_min;
}

void Texture::setWrap(WrapParameter param, WrapMode mode) {
    if (param == WRAP_S) {
        if (mode == d->wrap_s) return;
        d->wrap_s = mode;
        dirty();
    }
    else if (param == WRAP_T) {
        if (mode == d->wrap_t) return;
        d->wrap_t = mode;
        dirty();
    }
    else if (param == WRAP_R) {
        if (mode == d->wrap_r) return;
        d->wrap_r = mode;
        dirty();
    }
}

Texture::WrapMode Texture::getWrap(WrapParameter param) const {
    if (param == WRAP_S)
        return d->wrap_s;
    else if (param == WRAP_T)
        return d->wrap_t;
    else if (param == WRAP_R)
        return d->wrap_r;
    return d->wrap_s;
}
} // namespace glr