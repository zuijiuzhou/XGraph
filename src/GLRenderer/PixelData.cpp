
#include "PixelData.h"

#include <map>

#include "GraphicContext.h"
#include "State.h"

namespace glr {
VI_OBJECT_META_IMPL(PixelData, Buffer);

struct PixelData::Data {
    std::map<int, bool> dirties;
};

PixelData::PixelData()
  : d(new Data()) {
}
} // namespace glr