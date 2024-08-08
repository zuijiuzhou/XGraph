#include "CoordGenerator.h"
#include "ModelDefs.h"
#include <osg/AutoTransform>
#include <osg/Camera>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>

namespace glv {
namespace {
osg::ref_ptr<osg::Material> s_mat_red   = new osg::Material();
osg::ref_ptr<osg::Material> s_mat_green = new osg::Material();
osg::ref_ptr<osg::Material> s_mat_blue  = new osg::Material();

struct Initializer {
    Initializer() {
        s_mat_red->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 0, 0, 1));
        s_mat_red->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1, 0, 0, 1));
        s_mat_green->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 1, 0, 1));
        s_mat_green->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 1, 0, 1));
        s_mat_blue->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 1, 1));
        s_mat_blue->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0, 0, 1, 1));
    }
} initializer;
} // namespace

osg::MatrixTransform*
createCoord(double cyli_len, double cyli_r, double cone_len, double cone_r, bool auto_rotate_to_screen) {
    auto axis_cyli = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0, 0, cyli_len / 2.), cyli_r, cyli_len));
    auto axis_cone = new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0, 0, cyli_len + cone_len / 3.), cone_r, cone_len));
    auto axis_geod = new osg::Geode();
    axis_geod->addDrawable(axis_cyli);
    axis_geod->addDrawable(axis_cone);

    auto z_axis = new osg::MatrixTransform();
    z_axis->addChild(axis_geod);
    z_axis->getOrCreateStateSet()->setAttributeAndModes(s_mat_blue);
    auto x_axis = new osg::MatrixTransform();
    x_axis->addChild(axis_geod);
    x_axis->setMatrix(osg::Matrix::rotate(osg::PI_2, osg::Vec3d(0, 1, 0)));
    x_axis->getOrCreateStateSet()->setAttributeAndModes(s_mat_red);
    auto y_axis = new osg::MatrixTransform();
    y_axis->addChild(axis_geod);
    y_axis->setMatrix(osg::Matrix::rotate(-osg::PI_2, osg::Vec3d(1, 0, 0)));
    y_axis->getOrCreateStateSet()->setAttributeAndModes(s_mat_green);

    auto root = new osg::MatrixTransform();
    root->getOrCreateStateSet()->setMode(GL_NORMALIZE, 1);
    if (auto_rotate_to_screen) {
        auto at = new osg::AutoTransform();
        at->addChild(x_axis);
        at->addChild(y_axis);
        at->addChild(z_axis);
        at->setAutoScaleToScreen(true);
        root->addChild(at);
    }
    else {
        root->addChild(x_axis);
        root->addChild(y_axis);
        root->addChild(z_axis);
    }

    setPipelineMask(root, PipelineMask::FORWARD_SCENE_MASK);
    return root;
}

osg::Camera* createHudCoord(osg::Camera* master, double cyli_len, double cyli_r, double cone_len, double cone_r) {
    struct UpdateCallback : public osg::NodeCallback {
        UpdateCallback(osg::Camera* master)
          : master_(master) {}
        virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) {
            auto      cam = node->asCamera();
            osg::Vec3 eye, target, up, dir;
            master_->getViewMatrixAsLookAt(eye, target, up);
            dir = target - eye;
            dir.normalize();
            cam->setViewMatrixAsLookAt(-dir * 50, osg::Vec3(), up);

            auto vp_w = master_->getViewport()->width();
            auto vp_h = master_->getViewport()->height();

            cam->setViewport(vp_w - 200, 0, 200, 200);
            node->traverse(*nv);
        }
        osg::Camera* master_;
    };

    auto cam = new osg::Camera();
    cam->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    cam->addChild(createCoord(cyli_len, cyli_r, cone_len, cone_r, false));
    cam->setClearDepth(1.0);
    cam->setGraphicsContext(master->getGraphicsContext());
    cam->setClearMask(GL_DEPTH_BUFFER_BIT);
    cam->setRenderOrder(osg::Camera::POST_RENDER);
    cam->setProjectionMatrixAsOrtho(-100, 100, -100, 100, 1, 100);
    cam->addUpdateCallback(new UpdateCallback(master));
    return cam;
}
} // namespace glv