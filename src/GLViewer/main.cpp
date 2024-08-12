#include <filesystem>
#include <iostream>

#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>

#include "Utilities/Environment.h"
#include "Utilities/Text.h"

#include "BrepLoader.h"
#include "CoordGenerator.h"
#include "CurveGenerator.h"
#include "DottedCurve.h"
#include "Initializer.hpp"
#include "MeshCutterVTK.h"
#include "MeshLoader.h"
#include "ModelDefs.h"
#include "PointCloudLoader.h"
#include "Viewer.h"

int main(int argc, char** argv) {

    using namespace glv;

    osg::setNotifyLevel(osg::DEBUG_FP);

    glutils::Initializer initializer;
    initializer.initGlfw();
    initializer.initGlad();

    osg::Group* model = nullptr;
    if (argc == 1) {
        model = new osg::Group();
        std::vector<osg::Vec3d> cpts;
        std::vector<double>     params;
        std::vector<osg::Vec3d> tans;

        cpts.push_back(osg::Vec3d(0, 0, 100));
        cpts.push_back(osg::Vec3d(0, 10, 100));
        cpts.push_back(osg::Vec3d(0, 12, 90));
        cpts.push_back(osg::Vec3d(0, 14, 80));
        cpts.push_back(osg::Vec3d(0, 16, 70));
        cpts.push_back(osg::Vec3d(0, 18, 50));
        cpts.push_back(osg::Vec3d(0, 20, 40));
        cpts.push_back(osg::Vec3d(0, 30, 42));
        cpts.push_back(osg::Vec3d(0, 40, 30));
        cpts.push_back(osg::Vec3d(0, 50, 40));
        cpts.push_back(osg::Vec3d(0, 60, 43));
        cpts.push_back(osg::Vec3d(0, 70, 100));
        cpts.push_back(osg::Vec3d(0, 80, 110));
        cpts.push_back(osg::Vec3d(0, 90, 120));
        cpts.push_back(osg::Vec3d(0, 100, 130));
        cpts.push_back(osg::Vec3d(0, 110, 130));
        cpts.push_back(osg::Vec3d(0, 110, 100));
        cpts.push_back(osg::Vec3d(0, 100, 80));
        cpts.push_back(osg::Vec3d(0, 90, 60));
        cpts.push_back(osg::Vec3d(0, 60, 40));

        params.resize(cpts.size());
        for (int i = 0; i < cpts.size(); i++) {
            params[i] = i;
        }

        // tans.push_back(osg::Vec3d(0, 1, 0));
        // tans.push_back(osg::Vec3d(0, 1, 0));
        // model->addChild(createBSpline(cpts, params, tans, osg::Vec4(1, 0, 0, 1)));

        // for (int i = 0; i < cpts.size(); i++)
        // {
        //     params[i] = i * i;
        // }
        // model->addChild(createBSpline(cpts, params, tans, osg::Vec4(0, 1, 0, 1)));

        // params[0] = 0;
        // for (int i = 1; i < cpts.size(); i++)
        // {
        //     params[i] = sqrt(i);
        // }
        // model->addChild(createBSpline(cpts, params, tans, osg::Vec4(0, 0, 1, 1)));
        // model->addChild(createBSpline(cpts, {}, tans, osg::Vec4(0, 1, 1, 1)));

        tans.resize(cpts.size());
        std::vector<osg::Vec3d> temp_tans;
        temp_tans.resize(cpts.size());
        for (int i = 0; i < cpts.size(); i++) {
            if (i == cpts.size() - 1) {
                temp_tans[i] = temp_tans[i - 1];
            }
            else {
                auto& p0     = cpts[i];
                auto& p1     = cpts[i + 1];
                temp_tans[i] = (p1 - p0);
                // temp_tans[i].normalize();
            }
        };

        for (int i = 0; i < cpts.size(); i++) {
            if (i == 0) {
                tans[i] = temp_tans.front();
                tans[i].normalize();
            }
            else if (i == cpts.size() - 1) {
                tans[i] = temp_tans.back();
                tans[i].normalize();
            }
            else {
                auto& v1   = temp_tans[i - 1];
                auto& v2   = temp_tans[i];
                auto  v    = -v1 + v2;
                auto  len1 = v1.normalize();
                auto  len2 = v2.normalize();
                auto  len  = v.normalize();
                v          = -v1 + v2;
                // 同向或反向
                if (abs((v1 * v2) - 1.) < 1e-6) {
                    tans[i] = temp_tans[i];
                    continue;
                }

                cpts[i] += v * std::min(2., len * 0.1);
                tans[i] = v ^ (v2 ^ -v1);
            }
        }
        // model->addChild(createBSpline(cpts, {}, {}, osg::Vec4(1, 0, 1, 1)));

        // auto dc = createDottedCurve(cpts, {}, {});

        // auto dc_geo = dc->createGeometry(true, false, osg::Vec4(1, 0, 0, 1), osg::Vec4(0, 1, 0, 1));
        // model->addChild(dc_geo);
        // dc->downsample(10, osg::DegreesToRadians(150.0f), 8);

        // dc_geo = dc->createGeometry(false, true, osg::Vec4(1, 0, 0, 1), osg::Vec4(0, 1, 0, 1));
        // model->addChild(dc_geo);
        // delete dc;
    }
    else {
        std::string file = argv[1];
        if (MeshLoader::isSupported(file)) {
            model = MeshLoader().loadFile(file);
        }
        else if (BrepLoader::isSupported(file)) {
            model = BrepLoader().loadFile(file);
        }
        else if (PointCloudLoader::isSupported(file)) {
            model = PointCloudLoader().loadFile(file);
        }
        else {
            std::cout << std::endl << "Not supported." << std::endl;
            return -2;
        }

        if (!model) {
            std::cout << std::endl << "Load failed." << std::endl;
            return -3;
        }
    }

    namespace fs = std::filesystem;
    fs::current_path(stdutils::getApplicationDir());

    auto   coord = createCoord(100, 2, 20, 4, true);
    Viewer v;

    auto hud_coord = createHudCoord(v.getMasterCamera(), 60, 2, 12, 4);

    v.addNode(model);
    v.addNode(coord);
    v.addCamera(hud_coord, false, false);

    MeshCutterVTK mesh_cutter;
    mesh_cutter.setMesh("R:\\models\\0731-43#-right.stl");
    mesh_cutter.setPlane(osg::Vec3(0, 0, 30), osg::Vec3(0, 0, 1));
    mesh_cutter.update();

    osg::Vec3Array*  pnts = new osg::Vec3Array();
    std::vector<int> nparts;

    mesh_cutter.getOrderedPoints(*pnts, nparts);

    std::vector<osg::Vec3> pnts2;
    pnts2.reserve(pnts->size());

    for(int i = 0; i < nparts.front(); i++){

        pnts2.push_back(pnts->at(i));
    } 

    // auto dc = createDottedCurve(pnts2, {}, {});

    auto dc = new DottedCurve();
    dc->setInputPoints(pnts2);
    // dc->setIsClosed(true);
    dc->downsample(30, 120, 6);
    v.addNode(dc->createGeometry());

    auto pnts3 = dc->getPoints();

    auto dc2 = createDottedCurve(pnts3, {}, {});
    auto dc2_model = dc2->createGeometry();
    dc2_model->setMatrix(osg::Matrix::translate(0,0,-50));
    v.addNode(dc2_model);
    // v.addNode(mesh_cutter.createGeometry(true, true, osg::Vec4(1, 0, 0, 1), osg::Vec4(0, 1, 0, 1)));

    v.fitToScreen();
    v.run();

    return 0;
}