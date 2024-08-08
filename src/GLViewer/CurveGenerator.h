
#pragma once
#include <osg/Vec3d>
#include <osg/Vec4>
#include <vector>

namespace osg {
class MatrixTransform;
}
namespace glv {

class DottedCurve;

osg::MatrixTransform* createBSpline(const std::vector<osg::Vec3d>& ctrl_points,
                                    const std::vector<double>&     params,
                                    const std::vector<osg::Vec3d>& tangents,
                                    const osg::Vec4&               color);

DottedCurve* createDottedCurve(const std::vector<osg::Vec3d>& ctrl_points,
                              const std::vector<double>&     params,
                              const std::vector<osg::Vec3d>& tangents);
} // namespace glv