#include "RenderInfo.h"

#include "Viewer.h"

namespace glr {
VI_OBJECT_META_IMPL(RenderInfo, Object);

struct RenderInfo::Data {
    vine::RefPtr<Viewer> viewer;
};

RenderInfo::RenderInfo(Viewer* viewer)
  : d(new Data()) {
    d->viewer = viewer;
}

RenderInfo::~RenderInfo() {
    delete d;
}

Viewer* RenderInfo::getViewer() const {
    return d->viewer.get();
}
} // namespace glr