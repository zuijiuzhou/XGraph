#include "MeshCutterVTK.h"

#include <set>

#include <vtkActor.h>
#include <vtkCutter.h>
#include <vtkPlane.h>
#include <vtkPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkSTLReader.h>
#include <vtkSmartPointer.h>
#include <vtkStripper.h>

#include <osg/Depth>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Point>

namespace glv {

struct MeshCutterVTK::Data {
    osg::Vec3                    plane_origin;
    osg::Vec3                    plane_norm;
    std::string                  mesh_file;
    osg::ref_ptr<osg::Vec3Array> mesh_data;

    vtkSmartPointer<vtkPolyData> output_poly_data;
};

static inline bool isSamePoint(const osg::Vec3& p0, const osg::Vec3& p1) {
    return abs(p0.x() - p1.x()) < 1e-4 && abs(p0.y() - p1.y()) < 1e-4 && abs(p0.z() - p1.z() < 1e-4);
}

MeshCutterVTK::MeshCutterVTK()
  : d(new Data()) {
}

MeshCutterVTK::~MeshCutterVTK() {
    delete d;
}

void MeshCutterVTK::setPlane(const osg::Vec3& origin, const osg::Vec3& norm) {
    d->plane_origin = origin;
    d->plane_norm   = norm;
}

void MeshCutterVTK::setMesh(const std::string& file) {
    d->mesh_file = file;
}

void MeshCutterVTK::setMesh(osg::Vec3Array* vertices) {
    d->mesh_data = vertices;
}

void MeshCutterVTK::update() {
    d->output_poly_data = nullptr;
    vtkSmartPointer<vtkPolyData> polyData;
    if (d->mesh_data) {
    }
    else if (!d->mesh_file.empty()) {
        vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
        stlReader->SetFileName(d->mesh_file.data());
        stlReader->Update();

        polyData = stlReader->GetOutput();
    }

    if (!polyData) return;

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(d->plane_origin.x(), d->plane_origin.y(), d->plane_origin.z());
    plane->SetNormal(d->plane_norm.x(), d->plane_norm.y(), d->plane_norm.z());

    vtkSmartPointer<vtkCutter> cutter = vtkSmartPointer<vtkCutter>::New();
    cutter->SetInputData(polyData);
    cutter->SetCutFunction(plane);
    cutter->Update();

    vtkSmartPointer<vtkStripper> stripper = vtkSmartPointer<vtkStripper>::New();
    stripper->SetInputData(cutter->GetOutput());
    stripper->Update();

    d->output_poly_data = cutter->GetOutput();
}

void MeshCutterVTK::getPoints(osg::Vec3Array& pnts) const {
    pnts.clear();
    if (d->output_poly_data->GetNumberOfPoints() > 0) {
        double pt[3];
        for (int i = 0; i < d->output_poly_data->GetNumberOfPoints(); ++i) {
            d->output_poly_data->GetPoint(i, pt);
            pnts.push_back(osg::Vec3(pt[0], pt[1], pt[2]));
        }
    }
}

void MeshCutterVTK::getPoints(std::vector<osg::Vec3>& pnts) const {
    pnts.clear();
    if (d->output_poly_data->GetNumberOfPoints() > 0) {
        double pt[3];
        for (int i = 0; i < d->output_poly_data->GetNumberOfPoints(); ++i) {
            d->output_poly_data->GetPoint(i, pt);
            pnts.push_back(osg::Vec3(pt[0], pt[1], pt[2]));
        }
    }
}

void MeshCutterVTK::getOrderedPoints(osg::Vec3Array& pnts, std::vector<int>& parts) const {
    pnts.clear();
    parts.clear();

    struct Line {
        Line(const osg::Vec3& pt0, const osg::Vec3& pt1)
          : p0(pt0)
          , p1(pt1) {}

        osg::Vec3 p0;
        osg::Vec3 p1;
        Line*     prev = nullptr;
        Line*     next = nullptr;

        void reverse() {
            std::swap(p0.x(), p1.x());
            std::swap(p0.y(), p1.y());
            std::swap(p0.z(), p1.z());
        }

        bool setIfConnected(Line& other) {
            if (isSamePoint(p0, other.p0)) {

                if (!prev && !other.next) {
                    other.reverse();
                    prev       = &other;
                    other.next = this;
                }
                else if (!next && !other.prev) {
                    reverse();
                    next       = &other;
                    other.prev = this;
                }
                else if (!prev && !other.prev) {
                    auto pother = &other;
                    while (pother) {
                        pother->reverse();
                        std::swap(pother->prev, pother->next);
                        pother = pother->prev;
                    }
                    prev       = &other;
                    other.next = this;
                }
                else {
                    assert(false);
                }
                return true;
            }
            else if (isSamePoint(p0, other.p1)) {
                assert(other.next == nullptr && prev == nullptr);
                other.next = this;
                prev       = &other;
                return true;
            }
            else if (isSamePoint(p1, other.p0)) {
                assert(next == nullptr && other.prev == nullptr);
                next       = &other;
                other.prev = this;
                return true;
            }
            else if (isSamePoint(p1, other.p1)) {
                if (!next && !other.prev) {
                    other.reverse();
                    next       = &other;
                    other.prev = this;
                }
                else if (!prev && !other.next) {
                    reverse();
                    prev       = &other;
                    other.next = this;
                }
                else if (!next && !other.next) {
                    auto pother = &other;
                    while (pother) {
                        pother->reverse();
                        std::swap(pother->prev, pother->next);
                        pother = pother->next;
                    }
                    next       = &other;
                    other.prev = this;
                }
                else {
                    assert(false);
                }
                return true;
            }
            return false;
        }
    };

    vtkIdType         npts = 0;
    const vtkIdType*  pts  = nullptr;
    double            pt0[3];
    double            pt1[3];
    auto              vtklines = d->output_poly_data->GetLines();
    std::vector<Line> lines;
    std::list<Line*>  linelist;

    vtklines->InitTraversal();
    lines.reserve(d->output_poly_data->GetNumberOfLines());
    while (vtklines->GetNextCell(npts, pts)) {
        d->output_poly_data->GetPoint(pts[0], pt0);
        d->output_poly_data->GetPoint(pts[1], pt1);
        lines.emplace_back(osg::Vec3(pt0[0], pt0[1], pt0[2]), osg::Vec3(pt1[0], pt1[1], pt1[2]));
        linelist.push_back(&lines.back());
    }

    for (auto i = 0; i < lines.size() - 1; i++) {
        auto& line1 = lines[i];
        for (auto j = i + 1; j < lines.size(); j++) {
            auto& line2 = lines[j];
            line1.setIfConnected(line2);
        }
    }

    while (!linelist.empty()) {
        auto line0 = linelist.front();
        auto line  = line0;
        while (line->prev) {
            line = line->prev;

            // 闭合
            if (line == line0) {
                break;
            }
        }

        auto npnts = 1;
        pnts.push_back(line->p0);
        while (line) {
            auto iter = std::find(linelist.begin(), linelist.end(), line);
            if (iter == linelist.end()) {
                break;
            }
            else {
                linelist.erase(iter);
            }

            pnts.push_back(line->p1);
            ++npnts;
            line = line->next;
        }
        parts.push_back(npnts);
    }

    printf("Parts=%l", parts.size());

    // vtkNew<vtkPolyDataConnectivityFilter> filter;
    // filter->SetInputData(d->output_poly_data);
    // filter->SetExtractionModeToAllRegions();
    // filter->Update();

    // auto nregs = filter->GetNumberOfExtractedRegions();
    // for (int i = 0; i < nregs; i++) {
    //     filter->SetExtractionModeToSpecifiedRegions();
    //     filter->AddSpecifiedRegion(i);
    //     filter->Update();
    //     auto data   = filter->GetOutput();
    //     auto nlines = data->GetNumberOfLines();
    // }
}

void MeshCutterVTK::getOrderedPoints(std::vector<osg::Vec3>& pnts, std::vector<int>& parts) const {
    pnts.clear();
    parts.clear();
}

osg::MatrixTransform* MeshCutterVTK::createGeometry(bool             points_visible,
                                                    bool             lines_visible,
                                                    const osg::Vec4& points_color,
                                                    const osg::Vec4& lines_color) {
    auto vertices = new osg::Vec3Array();
    auto colors   = new osg::Vec4Array();
    auto geom     = new osg::Geometry();
    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_PER_PRIMITIVE_SET);
    if (lines_visible) {
        vertices->reserve(d->output_poly_data->GetNumberOfLines() * 2);

        auto lines = d->output_poly_data->GetLines();
        lines->InitTraversal();

        vtkIdType        npts = 0;
        const vtkIdType* pts  = nullptr;

        double pt[3];
        while (lines->GetNextCell(npts, pts)) {
            d->output_poly_data->GetPoint(pts[0], pt);
            vertices->push_back(osg::Vec3(pt[0], pt[1], pt[2]));
            d->output_poly_data->GetPoint(pts[1], pt);
            vertices->push_back(osg::Vec3(pt[0], pt[1], pt[2]));
        }

        colors->push_back(lines_color);
        geom->addPrimitiveSet(new osg::DrawArrays(GL_LINES, 0, vertices->size()));
    }
    if (points_visible) {
        colors->push_back(points_color);
        geom->addPrimitiveSet(new osg::DrawArrays(GL_POINTS, 0, vertices->size()));
    }

    auto geod = new osg::Geode();
    geod->addDrawable(geom);

    auto mt = new osg::MatrixTransform();
    mt->addChild(geod);
    mt->getOrCreateStateSet()->setAttributeAndModes(new osg::Point(3.0f));
    mt->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);
    // mt->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS));
    return mt;
}
} // namespace glv