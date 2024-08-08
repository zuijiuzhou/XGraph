#include "Viewer.h"

#include <glad/glad.h>

#include <osgDB/WriteFile>
#include <osgGA/StateSetManipulator>
#include <osgGA/TerrainManipulator>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgVerse/Pipeline/LightModule.h>
#include <osgVerse/Pipeline/Pipeline.h>
#include <osgVerse/Pipeline/ShadowModule.h>
#include <osgVerse/Pipeline/SkyBox.h>

namespace glv {

namespace {
class ViewerEx : public osgViewer::Viewer {
  public:
    ViewerEx(osgVerse::Pipeline* pipeline)
      : pipeline_(pipeline) {}

  protected:
    virtual osg::GraphicsOperation* createRenderer(osg::Camera* camera) {
        if (pipeline_.valid())
            return pipeline_->createRenderer(camera);
        else
            return osgViewer::Viewer::createRenderer(camera);
    }

  private:
    osg::ref_ptr<osgVerse::Pipeline> pipeline_;
};

constexpr const char* S_PICK_VS_STR = R"(
#version 330 core
layout(location = 0) in vec3 position;
// layout(location = 15) in vec4 color;
uniform mat4 osg_ModelViewProjectionMatrix;
out vec4 frag_color;
void main(){
  gl_Position = osg_ModelViewProjectionMatrix * vec4(position.x, position.y, position.z, 1.0);
  frag_color = vec4(1, 1, 0, 1);
}
  )";

constexpr const char* S_PICK_FS_STR = R"(
#version 330 core
in vec4 frag_color;
out vec4 FragColor;
void main(){
  FragColor = frag_color;
}
  )";

class PickCameraPostDrawCallback : public osg::Camera::DrawCallback {
  public:
    virtual void operator()(osg::RenderInfo& renderInfo) const {
        static bool is_first_frame = true;
        if (is_first_frame) {
            is_first_frame = false;
            auto cam       = renderInfo.getCurrentCamera();
            auto vp        = cam->getViewport();
            auto img       = new osg::Image();
            glBindFramebuffer(GL_FRAMEBUFFER, 1);
            GLint fbo_id;
            glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo_id);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            img->readPixels(0, 0, vp->width(), vp->height(), GL_GREEN, GL_UNSIGNED_BYTE);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            osgDB::writeImageFile(*img, "d:/1.bmp");
            img->unref();
        }
    }
};

osg::Camera* createPickCamera(PickCameraPostDrawCallback* postdraw) {

    auto prog = new osg::Program();
    prog->addShader(new osg::Shader(osg::Shader::VERTEX, S_PICK_VS_STR));
    prog->addShader(new osg::Shader(osg::Shader::FRAGMENT, S_PICK_FS_STR));

    auto cam = new osg::Camera();
    cam->setViewport(0, 0, 1280, 720);
    cam->setRenderOrder(osg::Camera::POST_RENDER);
    cam->setClearColor(osg::Vec4());
    cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    cam->getOrCreateStateSet()->setAttributeAndModes(prog, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    cam->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
    cam->setPostDrawCallback(postdraw);


    auto depth_buffer = new osg::RenderBuffer(1280, 720, GL_DEPTH_COMPONENT24);
    auto color_buffer = new osg::RenderBuffer(1280, 720, GL_RGBA8);
    // auto fbo = new osg::FrameBufferObject();
    // fbo->setAttachment(osg::Camera::DEPTH_BUFFER, osg::FrameBufferAttachment(depth_buffer));
    // fbo->setAttachment(osg::Camera::COLOR_BUFFER0, osg::FrameBufferAttachment(color_buffer));
    // cam->getOrCreateStateSet()->setAttributeAndModes(fbo);

    // auto depth_buffer = new osg::Texture2D();
    // depth_buffer->setInternalFormat(GL_DEPTH_COMPONENT24);
    // depth_buffer->setTextureSize(1280, 720);

    // auto color_buffer = new osg::Texture2D();
    // color_buffer->setInternalFormat(GL_RGBA8);


    // auto color_img = new osg::Image;
    // color_img->allocateImage(1280, 720, 1, GL_RGBA, GL_FLOAT);
    // color_img->setInternalTextureFormat(GL_RGBA8);
    // color_buffer->setImage(color_img);

    cam->attach(osg::Camera::DEPTH_BUFFER, GL_DEPTH_COMPONENT24);
    cam->attach(osg::Camera::COLOR_BUFFER0, GL_RGBA8);

    return cam;
}
} // namespace

struct Viewer::Rep {
    osg::ref_ptr<ViewerEx>   viewer_impl;
    osg::ref_ptr<osg::Group> root;
};

Viewer::Viewer()
  : rep_(new Rep()) {
    static bool is_verse_initialized = false;
    if (!is_verse_initialized) {
        osgVerse::globalInitialize(0, 0);
        is_verse_initialized = true;
    }
    auto pipeline     = new osgVerse::Pipeline();
    rep_->root        = new osg::Group();
    rep_->viewer_impl = new ViewerEx(pipeline);

    auto traits              = new osg::GraphicsContext::Traits();
    traits->x                = 100;
    traits->y                = 100;
    traits->width            = 1280;
    traits->height           = 720;
    traits->windowDecoration = true;
    traits->supportsResize   = true;
    traits->doubleBuffer     = true;
    traits->depth            = 24;
    traits->samples          = 4;
    traits->screenNum        = 0;
    traits->glContextVersion = "4.6";
    traits->windowName       = "ModelViewer";
    auto gc                  = osg::GraphicsContext::createGraphicsContext(traits);
    // false: gl_Vertex=0,gl_Normal=2,gl_Color=3
    // true : gl_Vertex=0,gl_Normal=1,gl_Color=2
    // gc->getState()->resetVertexAttributeAlias(false);
    // gc->getState()->setUseVertexAttributeAliasing(true);
    gc->getState()->setUseModelViewAndProjectionUniforms(true);
    // gc->getState()->setUseVertexAttributeAliasing(true);

    auto cam = rep_->viewer_impl->getCamera();
    cam->setGraphicsContext(gc);
    cam->setViewport(0, 0, traits->width, traits->height);
    cam->setProjectionMatrixAsPerspective(30, (double)traits->width / traits->height, 1, 1000);
    cam->setViewMatrixAsLookAt(osg::Vec3(200, 0, 0), osg::Vec3(), osg::Vec3(0, 1, 0));

    auto camm = new osgGA::TrackballManipulator(osgGA::StandardManipulator::DEFAULT_SETTINGS |
                                                osgGA::StandardManipulator::SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT);
    // auto camm = new osgGA::TerrainManipulator();
    camm->setAutoComputeHomePosition(false);
    camm->setByMatrix(cam->getViewMatrix());

    auto pick_cam = createPickCamera(new PickCameraPostDrawCallback());
    pick_cam->setGraphicsContext(gc);
    addCamera(pick_cam, true, true, true, true);

    rep_->viewer_impl->setCameraManipulator(camm);
    rep_->viewer_impl->addEventHandler(new osgViewer::StatsHandler());
    rep_->viewer_impl->addEventHandler(new osgViewer::WindowSizeHandler());
    rep_->viewer_impl->addEventHandler(
        new osgGA::StateSetManipulator(rep_->viewer_impl->getCamera()->getOrCreateStateSet()));
    rep_->viewer_impl->setThreadingModel(osgViewer::Viewer::SingleThreaded);
    rep_->viewer_impl->setSceneData(rep_->root);


    // Main light
    auto light0 = new osgVerse::LightDrawable;
    light0->setColor(osg::Vec3(1.0f, 1.0f, 1.0f));
    light0->setDirection(osg::Vec3(0.02f, 0.1f, -1.0f));
    light0->setDirectional(true);
    light0->setEyeSpace(false);

    auto light1 = new osgVerse::LightDrawable;
    light1->setColor(osg::Vec3(1.5f, 1.5f, 1.5f));
    light1->setDirection(osg::Vec3(1.f, 0.1f, 0.0f));
    light1->setDirectional(true);
    light1->setEyeSpace(false);

    auto lightGeode = new osg::Geode;
    lightGeode->addDrawable(light0);
    // lightGeode->addDrawable(light1);
    addNode(lightGeode);

    class ViewerEventCallback : public osgGA::GUIEventHandler {
      public:
        ViewerEventCallback(osgVerse::LightDrawable* light0)
          : light0_(light0) {}
        virtual bool handle(const osgGA::GUIEventAdapter& ea,
                            osgGA::GUIActionAdapter&      aa,
                            osg::Object*,
                            osg::NodeVisitor*) override {
            if (ea.getEventType() == osgGA::GUIEventAdapter::FRAME) {
                osg::Vec3 eye, center, up, dir;
                aa.asView()->getCamera()->getViewMatrixAsLookAt(eye, center, up);
                // dir = center - eye;
                dir = -eye;
                dir.normalize();
                // dir = dir * osg::Matrix::rotate(osg::PI_4 / 4, up);
                light0_->setDirection(dir);
            }
            return false;
        }

      private:
        osgVerse::LightDrawable* light0_;
    };

    rep_->viewer_impl->addEventHandler(new ViewerEventCallback(light0));

    osgVerse::StandardPipelineParameters params(SHADER_DIR, SKYBOX_DIR "barcelona.hdr");
    params.enablePostEffects = true;
    params.enableAO          = false;
    // osgVerse::setupStandardPipeline(pipeline, rep_->viewer_impl.get(), params);

    // // Post pipeline settings
    // auto shadow = static_cast<osgVerse::ShadowModule*>(pipeline->getModule("Shadow"));
    // if (shadow && shadow->getFrustumGeode())
    // {
    //     addNode(shadow->getFrustumGeode(), FORWARD_SCENE_MASK);
    // }

    auto light = static_cast<osgVerse::LightModule*>(pipeline->getModule("Light"));
    if (light) light->setMainLight(light0, "Shadow");
}

void Viewer::run() {
    rep_->viewer_impl->run();
}

void Viewer::addNode(osg::Node* node) {
    osgVerse::TangentSpaceVisitor tsv;
    node->accept(tsv);
    rep_->root->addChild(node);
}

void Viewer::addCamera(osg::Camera* cam,
                       bool         useMasterSceneData,
                       bool         useMasterViewMatrix,
                       bool         useMasterProjMatrix,
                       bool         useMasterViewport) {
    rep_->viewer_impl->addSlave(cam, useMasterSceneData);

    if (useMasterViewMatrix || useMasterProjMatrix || useMasterViewport) {
        struct UpdateCallback : osg::NodeCallback {
            UpdateCallback(osg::Camera* master,
                           osg::Camera* slave,
                           bool         useMasterViewMatrix,
                           bool         useMasterProjMatrix,
                           bool         useMasterViewport)
              : master_(master)
              , slave_(slave)
              , use_mas_view_(useMasterViewMatrix)
              , use_mas_proj_(useMasterProjMatrix)
              , use_mas_vp_(useMasterViewport) {}

            osg::Camera *master_, *slave_;
            bool         use_mas_view_, use_mas_proj_, use_mas_vp_;

            virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) override {
                auto cam = node->asCamera();
                if (use_mas_view_) {
                    cam->setViewMatrix(master_->getViewMatrix());
                }
                if (use_mas_proj_) {
                    cam->setViewMatrix(master_->getProjectionMatrix());
                }
                if (use_mas_vp_) {
                    auto vp = master_->getViewport();
                    cam->setViewport(vp->x(), vp->y(), vp->width(), vp->height());
                }
            }
        };

        cam->addUpdateCallback(
            new UpdateCallback(getMasterCamera(), cam, useMasterViewMatrix, useMasterProjMatrix, useMasterViewport));
    }
}

void Viewer::fitToScreen() {
    auto cm = rep_->viewer_impl->getCameraManipulator();
    cm->computeHomePosition(rep_->viewer_impl->getCamera());
    cm->home(0);
}

osg::Camera* Viewer::getMasterCamera() const {
    return rep_->viewer_impl->getCamera();
}
} // namespace glv