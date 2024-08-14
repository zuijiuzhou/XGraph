#include <QApplication>
#include <Windows.h>
#include <glm/glm.hpp>
#include <iostream>

#include <vine/core/Ptr.h>
#include <vine/ge/Rect2d.h>

#include "StdUtils/Resources.h"

#include "AppInitializer.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Geometry.h"
#include "GlfwViewer.h"
#include "GraphicContext.h"
#include "Image.h"
#include "Light.h"
#include "Material.h"
#include "MeshLoader.h"
#include "Model.h"
#include "PointCloudLoader.h"
#include "QtMainWindow.h"
#include "QtViewer.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Shader.h"
#include "SkyBox.h"
#include "StateSet.h"
#include "Texture2D.h"
#include "Uniform.h"
#include "Viewer.h"

void CreateSampleShapes(glr::Scene* scene) {
    auto axis = new glr::Model();
    {
        auto geom     = new glr::Geometry();
        auto vertices = new glr::Vec3fArray();
        {
            vertices->push_back(glm::vec3());
            vertices->push_back(glm::vec3(10, 0, 0));
            vertices->push_back(glm::vec3());
            vertices->push_back(glm::vec3(0, 10, 0));
            vertices->push_back(glm::vec3());
            vertices->push_back(glm::vec3(0, 0, 10));
        }
        geom->addVertexAttribArray(0, vertices);
        auto colors = new glr::Vec4fArray();
        {
            colors->emplace_back(1.f, 0.f, 0.0f, 1.0f);
            colors->emplace_back(1.f, 0.f, 0.0f, 1.0f);
            colors->emplace_back(0.f, 1.f, 0.0f, 1.0f);
            colors->emplace_back(0.f, 1.f, 0.0f, 1.0f);
            colors->emplace_back(0.f, 0.f, 1.0f, 1.0f);
            colors->emplace_back(0.f, 0.f, 1.0f, 1.0f);
        }
        geom->addVertexAttribArray(2, colors);
        geom->addPrimitiveSet(new glr::DrawArrays(glr::PrimitiveSet::MODE_LINES, 0, vertices->size()));
        axis->addDrawable(geom);
        axis->getOrCreateStateSet()->setAttribute(new glr::Material());
        axis->getOrCreateStateSet()->setShader(
            glr::ResourceManager::instance()->getInternalShader(glr::ResourceManager::IS_Base));
        axis->getOrCreateStateSet()->setAttribute(new glr::Uniform("use_texture", false));
    }

    auto cube = new glr::Model();
    {
        auto geom   = glr::Geometry::createCube(1, 0, 1, -1, 3);
        auto colors = new glr::Vec4fArray();
        colors->emplace_back(0.8f, 0.8f, 0.8f, 1.0f);
        geom->addVertexAttribArray(2, colors);
        auto tex = glr::ResourceManager::instance()->getInternalCubeMap(glr::ResourceManager::ICM_CubeMap1);
        geom->addTexture(GL_TEXTURE0, "tex", tex);

        auto light = new glr::Light();
        light->setPosition(glm::vec4(10, 10, 10, 1.));
        light->setDirection(glm::vec3(2, 4, -1));
        light->setIsHead(true);
        auto lights = new glr::Lights();
        lights->addLight(light);

        cube->addDrawable(geom);
        glm::mat4 m1(1.f);
        m1 = glm::rotate(m1, glm::radians(90.f), glm::vec3(1.0f, 0.f, 0.f));
        cube->setMatrix(m1);
        cube->getOrCreateStateSet()->setAttribute(new glr::Material());
        cube->getOrCreateStateSet()->setAttribute(lights);
        cube->getOrCreateStateSet()->setShader(
            glr::ResourceManager::instance()->getInternalShader(glr::ResourceManager::IS_Geometry));
    }

    auto skybox =
        glr::createSkyBox(glr::ResourceManager::instance()->getInternalCubeMap(glr::ResourceManager::ICM_CubeMap2));

    auto pc = new glr::Model();
    {
        pc->getOrCreateStateSet()->setShader(
            glr::ResourceManager::instance()->getInternalShader(glr::ResourceManager::IS_PointCloud));
        auto geom     = new glr::Geometry();
        auto vertices = new glr::Vec3fArray();
        auto colors   = new glr::Vec3fArray();
        vertices->reserve(1000);
        colors->reserve(vertices->capacity());
        auto posi_offset = INT16_MAX / 10000.f / 2.f;
        for (size_t i = 0; i < vertices->capacity(); i++) {
            vertices->emplace_back(rand() / 10000. - posi_offset, rand() / 10000. - posi_offset, rand() / 10000.);
            colors->emplace_back(rand() / static_cast<double>(INT16_MAX),
                                 rand() / static_cast<double>(INT16_MAX),
                                 rand() / static_cast<double>(INT16_MAX));
        }
        geom->addVertexAttribArray(0, vertices);
        geom->addVertexAttribArray(1, colors);
        geom->addPrimitiveSet(new glr::DrawArrays(glr::DrawArrays::MODE_POINTS, 0, vertices->size()));
        pc->addDrawable(geom);
    }


    auto img = new glr::Model();
    {
        auto tex = new glr::Texture2D();
        tex->setImage("C:\\Users\\sa\\Downloads\\1.jpg");
        auto geom_img =
            glr::Geometry::createTexturedQuad(0,
                                              1,
                                              3,
                                              vine::ge::Rect2d(0, 0, tex->getWidth() / 100., tex->getHeight() / 100.),
                                              vine::ge::Rect2d(0, 0, 1, 1));

        // auto colors = new glr::Vec4fArray();
        // colors->push_back(glm::vec4(0,1,0,1));
        // geom_img->addVertexAttribArray(2, colors);

        geom_img->addTexture(0, "tex", tex);
        img->addDrawable(geom_img);
        img->getOrCreateStateSet()->setAttribute(new glr::Uniform("use_texture", true));
        img->getOrCreateStateSet()->setShader(
            glr::ResourceManager::instance()->getInternalShader(glr::ResourceManager::IS_Base));
    }

    scene->addModel(axis);
    scene->addModel(pc);
    scene->addModel(cube);
    scene->addModel(skybox);
    scene->addModel(img);
}

int main(int argc, char** argv) {
    glr::AppInitializationParameters params;
    glr::AppInitializer              initializer(params);
    initializer.initGlfw();
    initializer.initGlad();
    initializer.initQt();

    auto scene = new glr::Scene();

#define GLFW_VIEWER_

#ifdef GLFW_VIEWER
    glr::GlfwViewer v;
    v.initialize();
    auto renderer = v.getViewer()->getMasterRenderer();
#else
    // QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication      app(argc, argv);
    glr::QtMainWindow wnd;
    auto              renderer = wnd.getViewer()->getViewer()->getMasterRenderer();
#endif

    if (argc > 1) {
        auto file = argv[1];
        // auto pcl = new glr::PointCloudLoader(file);
        // renderer->addModel(pcl->getData());

        auto model = glr::MeshLoader().loadFile(file);
        auto light = new glr::Light();
        light->setPosition(glm::vec4(10, 10, 10, 1.));
        light->setDirection(glm::vec3(2, 4, -1));
        light->setIsHead(true);
        auto lights = new glr::Lights();
        lights->addLight(light);
        model->getOrCreateStateSet()->setAttribute(new glr::Material());
        model->getOrCreateStateSet()->setAttribute(lights);
        model->getOrCreateStateSet()->setAttribute(new glr::Uniform("use_texture", false));
        model->getOrCreateStateSet()->setShader(
            glr::ResourceManager::instance()->getInternalShader(glr::ResourceManager::IS_Base));
        scene->addModel(model);
    }
    else {
        CreateSampleShapes(scene);
    }

    auto x = renderer->isKindOf<vine::Object>();
    auto y = renderer->toString();
    std::cout << 1 << std::endl;

#ifdef GLFW_VIEWER
    renderer->setScene(scene);
    v.run();
#else
    renderer->setScene(scene);
    wnd.show();
    app.exec();
#endif
    return 0;
}