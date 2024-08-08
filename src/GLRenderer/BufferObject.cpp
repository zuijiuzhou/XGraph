#include "BufferObject.h"

#include <map>

#include "GraphicContext.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(BufferObject, Buffer);

struct BufferObject::Data {
    std::map<int, bool> dirties;
};

BufferObject::BufferObject()
  : d(new Data()) {
}
} // namespace glr