#pragma once

#include "glrenderer_global.h"

#include <map>
#include <set>
#include <string>
#include <vector>

#include <vine/ge/ge_global.h>

#include "Arrays.h"
#include "Drawable.h"
#include "PrimitiveSet.h"

VI_GE_NS_BEGIN
class Rect2d;
VI_GE_NS_END

namespace glr {
class Texture;
class CubeMap;

class GLRENDERER_API Geometry : public Drawable {
    VI_OBJECT_META;

  public:
    Geometry();
    virtual ~Geometry();

  public:
    void draw(State& ctx) override;
    void addVertexAttribArray(GLuint loc, Array* data);
    void addTexture(GLuint unit, GLuint loc, Texture* tex);
    void addTexture(GLuint unit, const std::string& name, Texture* tex);
    void addPrimitiveSet(PrimitiveSet* prim);

    virtual BoundingBox getBoundingBox() const override;
    void                setBoundingBox(const BoundingBox& bb);

  public:
    static Geometry* createCube(float size,
                                int   vertices_loc,
                                int   normals_loc         = -1,
                                int   tex_2d_coords_loc   = -1,
                                int   cube_map_coords_loc = -1);
    static Geometry* createTexturedQuad(int                     vertices_loc,
                                        int                     norms_loc,
                                        int                     tex_coords_loc,
                                        const vine::ge::Rect2d& rect,
                                        const vine::ge::Rect2d& uv_rect);

  private:
    VI_OBJECT_DATA;
};
} // namespace glr
