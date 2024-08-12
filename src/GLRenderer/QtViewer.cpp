#include <glad/glad.h>

#include "QtViewer.h"

#include <QApplication>
#include <QMouseEvent>
#include <QOpenGLContext>

#include <vine/core/Ptr.h>

#include "Camera.h"
#include "CameraManipulator.h"
#include "Event.h"
#include "GraphicContext.h"
#include "Renderer.h"
#include "Viewer.h"

namespace glr {
namespace {
class GraphicContextQt : public GraphicContext {
  public:
    GraphicContextQt(QOpenGLWidget* widget)
      : widget_(widget) {}

    virtual ~GraphicContextQt() {}

  public:
    virtual int getWidth() const { return widget_->width(); }

    virtual int getHeight() const { return widget_->height(); }

  private:
    QOpenGLWidget* widget_ = nullptr;
};
} // namespace

struct QtViewer::Data {
    vine::RefPtr<Viewer>         viewer;
    vine::RefPtr<Renderer>       renderer;
    vine::RefPtr<GraphicContext> ctx;
};

QtViewer::QtViewer()
  : d(new Data()) {
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    // QSurfaceFormat format;
    // format.setProfile(QSurfaceFormat::CoreProfile);
    // format.setSamples(4);
    // format.setVersion(4, 6);
    // format.setDepthBufferSize(24);
    // format.setStencilBufferSize(8);
    // format.setColorSpace(QSurfaceFormat::ColorSpace::DefaultColorSpace);
    // format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
    // setFormat(format);

    auto viewer   = new Viewer();
    auto renderer = new Renderer();
    auto ctx      = new GraphicContextQt(this);
    auto cam      = renderer->getCamera();
    auto cm       = new StandardCameraManipulator(cam);
    renderer->setContext(ctx);
    renderer->setCameraManipulator(cm);
    viewer->addRenderer(renderer);

    d->viewer   = viewer;
    d->renderer = renderer;
    d->ctx      = ctx;
}

QtViewer::~QtViewer() {
    delete d;
}

Viewer* QtViewer::getViewer() const {
    return d->viewer.get();
}

void QtViewer::initializeGL() {
    // static auto ctx            = context();
    // static bool is_glad_loaded = false;
    // if (!is_glad_loaded) {
    //     if (gladLoadGLLoader((GLADloadproc)[](const char* name) { return (void*)ctx->getProcAddress(name); }))
    //         is_glad_loaded = true;
    //     else
    //         throw std::exception("GLAD init faild.");
    // }

    d->ctx->realize();

    auto cam = d->renderer->getCamera();
    cam->setViewport(0, 0, width(), height());
    cam->setClearDepth(1.0);
    cam->setClearStencil(1);
    cam->setClearColor(glm::vec4(0., 0., 0., 1.));
    cam->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_PROGRAM_POINT_SIZE);
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glFrontFace(GL_CCW);
    glDepthFunc(GL_LESS);
}

void QtViewer::resizeGL(int w, int h) {
    auto ratio = this->screen()->devicePixelRatio();
    d->ctx->notify(Event::createResizeEvent(d->ctx.get(), w * ratio, h * ratio));
}

void QtViewer::paintGL() {
    d->viewer->frame();
}

void QtViewer::resizeEvent(QResizeEvent* e) {
    QOpenGLWidget::resizeEvent(e);
}

void QtViewer::mousePressEvent(QMouseEvent* event) {
    auto btn = ButtonLeft;
    if (event->button() == Qt::LeftButton) {
        btn = ButtonLeft;
    }
    else if (event->button() == Qt::MiddleButton) {
        btn = ButtonMiddle;
    }
    else if (event->button() == Qt::RightButton) {
        btn = ButtonRight;
    }
    auto ratio = this->screen()->devicePixelRatio();
    d->ctx->notify(Event::createMousePressEvent(d->ctx.get(), btn, event->x() * ratio, event->y() * ratio));
    QOpenGLWidget::mousePressEvent(event);
    this->update();
};

void QtViewer::mouseReleaseEvent(QMouseEvent* event) {
    auto btn = ButtonLeft;
    if (event->button() == Qt::LeftButton) {
        btn = ButtonLeft;
    }
    else if (event->button() == Qt::MiddleButton) {
        btn = ButtonMiddle;
    }
    else if (event->button() == Qt::RightButton) {
        btn = ButtonRight;
    }
    auto ratio = this->screen()->devicePixelRatio();
    d->ctx->notify(Event::createMouseReleaseEvent(d->ctx.get(), btn, event->x() * ratio, event->y() * ratio));
    QOpenGLWidget::mouseReleaseEvent(event);
};

void QtViewer::mouseDoubleClickEvent(QMouseEvent* event) {
    QOpenGLWidget::mouseDoubleClickEvent(event);
    this->update();
};

void QtViewer::mouseMoveEvent(QMouseEvent* event) {
    auto ratio = this->screen()->devicePixelRatio();
    d->ctx->notify(Event::createMouseMoveEvent(d->ctx.get(), event->x() * ratio, event->y() * ratio));
    QOpenGLWidget::mouseMoveEvent(event);
    this->update();
};

void QtViewer::wheelEvent(QWheelEvent* event) {
    d->ctx->notify(Event::createMouseWheelEvent(d->ctx.get(), event->angleDelta().y()));
    QOpenGLWidget::wheelEvent(event);
    this->update();
};
} // namespace glr