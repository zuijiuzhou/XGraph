#include "PixelBufferObject.h"

#include <map>

#include "GraphicContext.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(PixelBufferObject, BufferObject);

struct PixelBufferObject::Data {
    std::map<int, bool> dirties;
};

PixelBufferObject::PixelBufferObject()
  : d(new Data()) {
}
} // namespace glr