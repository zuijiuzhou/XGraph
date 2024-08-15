#pragma once

#include "glrenderer_global.h"

namespace glr {
class Model;
class CubeMap;
GLRENDERER_API Model* createSkyBox(CubeMap* tex);
} // namespace glr