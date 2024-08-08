#pragma once
#include <vector>

#include <osg/Vec3>
#include <osg/Vec4>

namespace osg {
class MatrixTransform;
}

namespace glv {
class DottedCurve {
  public:
    DottedCurve();
    virtual ~DottedCurve();

  private:
    DottedCurve(const DottedCurve&)            = delete;
    DottedCurve(DottedCurve&&)                 = delete;
    DottedCurve& operator=(const DottedCurve&) = delete;

  public:
    void                   setInputPoints(const std::vector<osg::Vec3>& pts);
    void                   setInputPoints(std::vector<osg::Vec3>&& pts);
    void                   setIsClosed(bool val);
    void                   sort();
    void                   downsample(float max_dist, float max_angle, int max_iterations);
    std::vector<osg::Vec3> getPoints() const;
    osg::MatrixTransform*  createGeometry(const osg::Vec4& points_color, const osg::Vec4& lines_color, bool points_visible = true, bool lines_visible = true) const;

  private:
    struct Data;
    Data* const d;
};
} // namespace glv