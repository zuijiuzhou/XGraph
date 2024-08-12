
#include "CurveGenerator.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Point>

#include <GCPnts_UniformAbscissa.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Standard_Handle.hxx>
#include <TColStd_HArray1OfBoolean.hxx>
#include <TColStd_HArray1OfReal.hxx>
#include <TColgp_HArray1OfPnt.hxx>
#include <TColgp_HArray1OfVec.hxx>

#include "DottedCurve.h"

namespace glv {

static opencascade::handle<Geom_BSplineCurve> createCurve(const std::vector<osg::Vec3>& ctrl_points,
                                                          const std::vector<double>&    params,
                                                          const std::vector<osg::Vec3>& tangents) {
    Handle(TColgp_HArray1OfPnt) pnts           = new TColgp_HArray1OfPnt(1, ctrl_points.size());
    Handle(TColStd_HArray1OfReal) pnt_params   = new TColStd_HArray1OfReal(1, params.size());
    Handle(TColgp_HArray1OfVec) tans           = new TColgp_HArray1OfVec(1, tangents.size());
    Handle(TColStd_HArray1OfBoolean) tan_flags = new TColStd_HArray1OfBoolean(1, tangents.size());

    for (int i = 1; i <= ctrl_points.size(); i++) {
        auto& pt = ctrl_points[i - 1];
        (*pnts).SetValue(i, gp_Pnt(pt.x(), pt.y(), pt.z()));
    }
    for (int i = 1; i <= params.size(); i++) {
        (*pnt_params).SetValue(i, params[i - 1]);
    }

    for (int i = 1; i <= tangents.size(); i++) {
        auto& tan = tangents[i - 1];
        (*tans).SetValue(i, gp_Vec(tan.x(), tan.y(), tan.z()));
        (*tan_flags).SetValue(i, true);
    }
    std::shared_ptr<GeomAPI_Interpolate> interpolate;
    if (pnt_params->IsEmpty()) {
        interpolate = std::shared_ptr<GeomAPI_Interpolate>(new GeomAPI_Interpolate(pnts, false, 0.01));
    }
    else {
        interpolate = std::shared_ptr<GeomAPI_Interpolate>(new GeomAPI_Interpolate(pnts, pnt_params, false, 0.01));
    }
    GeomAPI_Interpolate(pnts, pnt_params, false, 0.01);
    if (ctrl_points.size() == tangents.size()) {
        interpolate->Load(*tans, tan_flags, false);
    }
    else if (tangents.size() == 2) {
        interpolate->Load(tans->First(), tans->Last(), false);
    }
    interpolate->Perform();
    if (interpolate->IsDone()) {
        auto curve = interpolate->Curve();
        return curve;
    }
    return nullptr;
}

static osg::Geometry* createCurveGeometry(Geom_BSplineCurve* curve, const osg::Vec4& color) {
    GeomAdaptor_Curve      adaptor(curve, curve->FirstParameter(), curve->LastParameter());
    GCPnts_UniformAbscissa spliter(adaptor, 1.);

    auto nb_pnts  = spliter.NbPoints();
    auto vertices = new osg::Vec3Array();
    vertices->reserve(nb_pnts);
    for (int i = 1; i <= nb_pnts; i++) {
        auto   param = spliter.Parameter(i);
        gp_Pnt posi;
        gp_Vec tangent;
        adaptor.D1(param, posi, tangent);
        vertices->push_back(osg::Vec3(posi.X(), posi.Y(), posi.Z()));
    }
    auto colors = new osg::Vec4Array();
    { colors->push_back(color); }
    auto geom = new osg::Geometry();
    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);
    geom->addPrimitiveSet(new osg::DrawArrays(GL_LINE_STRIP, 0, nb_pnts));
    return geom;
}

static osg::Geometry* createPointsGeometry(const std::vector<osg::Vec3>& points) {
    auto vertices = new osg::Vec3Array();
    for (auto& pt : points) {
        vertices->push_back(pt);
    }
    auto colors = new osg::Vec4Array();
    { colors->push_back(osg::Vec4(1, 1, 0, 1)); }
    auto geom = new osg::Geometry();
    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_OVERALL);
    geom->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, points.size()));
    geom->getOrCreateStateSet()->setAttributeAndModes(new osg::Point(4));
    return geom;
}

osg::MatrixTransform* createBSpline(const std::vector<osg::Vec3>& ctrl_points,
                                    const std::vector<double>&    params,
                                    const std::vector<osg::Vec3>& tangents,
                                    const osg::Vec4&              color) {
    auto curve = createCurve(ctrl_points, params, tangents);
    auto geod  = new osg::Geode();
    if (curve) {
        geod->addDrawable(createCurveGeometry(curve.get(), color));
    }

    geod->addDrawable(createPointsGeometry(ctrl_points));

    auto model = new osg::MatrixTransform();
    model->addChild(geod);
    model->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);
    return model;
}

DottedCurve* createDottedCurve(const std::vector<osg::Vec3>& ctrl_points,
                               const std::vector<double>&    params,
                               const std::vector<osg::Vec3>& tangents) {
    auto                   curve = createCurve(ctrl_points, params, tangents);
    GeomAdaptor_Curve      adaptor(curve, curve->FirstParameter(), curve->LastParameter());
    GCPnts_UniformAbscissa spliter(adaptor, 1.);

    auto                   nb_pnts = spliter.NbPoints();
    std::vector<osg::Vec3> pts;
    pts.reserve(nb_pnts);
    for (int i = 1; i <= nb_pnts; i++) {
        auto   param = spliter.Parameter(i);
        gp_Pnt posi;
        gp_Vec tangent;
        adaptor.D1(param, posi, tangent);
        pts.push_back(osg::Vec3(posi.X(), posi.Y(), posi.Z()));
    }
    auto dc = new DottedCurve();
    dc->setInputPoints(std::move(pts));
    return dc;
}
} // namespace glv