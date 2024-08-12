#pragma once

#include <vector>

#include <osg/Array>
#include <osg/Vec3>
#include <osg/vec4>

namespace osg {
class MatrixTransform;
}

namespace glv {
class MeshCutterVTK {
  public:
    MeshCutterVTK(/* args */);
    virtual ~MeshCutterVTK();

  public:
    void setPlane(const osg::Vec3& origin, const osg::Vec3& norm);
    void setMesh(const std::string& file);
    void setMesh(osg::Vec3Array* vertices);
    void update();
    void getPoints(osg::Vec3Array& pnts) const;
    void getPoints(std::vector<osg::Vec3>& pnts) const;
    void getOrderedPoints(osg::Vec3Array& pnts, std::vector<int>& parts) const;
    void getOrderedPoints(std::vector<osg::Vec3>& pnts, std::vector<int>& parts) const;

    osg::MatrixTransform* createGeometry(bool             points_visible = true,
                                         bool             lines_visible  = true,
                                         const osg::Vec4& points_color   = osg::Vec4(1, 0, 0, 1),
                                         const osg::Vec4& lines_color    = osg::Vec4(0, 1, 0, 1));

  private:
    struct Data;
    Data* const d;
};
} // namespace glv