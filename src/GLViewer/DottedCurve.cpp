#include "DottedCurve.h"

#include <algorithm>
#include <list>

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Point>
#include <osg/Vec4>

namespace glv {
struct DottedCurve::Data {
    std::vector<osg::Vec3> pts;
    bool                   is_closed  = false;
    bool                   is_sampled = false;
};

static inline float calcAngle(const osg::Vec4& pt1, const osg::Vec4& pt2, const osg::Vec4& pt3) {
    osg::Vec3 v1(pt1.x() - pt2.x(), pt1.y() - pt2.y(), pt1.z() - pt2.z());
    osg::Vec3 v2(pt3.x() - pt2.x(), pt3.y() - pt2.y(), pt3.z() - pt2.z());
    auto      dot   = v1 * v2;
    auto      len12 = v1.length() * v2.length();
    if (len12 < 1e-6) {
        return 1;
    }
    return dot / len12;
}

static void downsample(const std::vector<osg::Vec4>& input,
                       bool                          is_closed,
                       float                         min_cos,
                       float                         max_dist_squared,
                       std::vector<osg::Vec4>&       output) {
    output.clear();

    if (input.empty()) return;

    auto is_discarded = false;
    for (int i = 0; i < (is_closed ? input.size() : input.size() - 1); i++) {
        auto& pt = input[i];

        if (is_discarded) {
            output.push_back(pt);
            is_discarded = false;
            continue;
        }

        if (max_dist_squared != 0.0f) {
            auto& next_pt = input[i + 1];
            auto  dist    = (next_pt - pt).length2();
            if (dist > max_dist_squared) {
                output.push_back(pt);
                is_discarded = false;
                continue;
            }
        }

        if (pt.w() < min_cos) {
            const osg::Vec4* prev_pt      = nullptr;
            const osg::Vec4* prev_prev_pt = nullptr;
            const osg::Vec4* next_pt      = nullptr;
            const osg::Vec4* next_next_pt = nullptr;

            if (i == 0) {
                prev_pt      = is_closed ? &input.back() : nullptr;
                prev_prev_pt = is_closed ? &input[input.size() - 2] : nullptr;
            }
            else {
                prev_pt = &input[i - 1];
                if (i == 1) {
                    prev_prev_pt = is_closed ? &input.back() : nullptr;
                }
                else {
                    prev_prev_pt = &input[i - 2];
                }
            }

            if(i == input.size() - 1){
                next_pt = is_closed ? &input.front() : nullptr;
                next_next_pt = is_closed ? &input[1] : nullptr;
            }
            else{
                next_pt = &input[i + 1];
                if(i == input.size() - 2){
                    next_next_pt = is_closed ? &input.front() : nullptr;
                }
                else{
                    next_next_pt = &input[i+2];
                }
            }

            if(prev_pt && prev_prev_pt && next_pt && calcAngle(*prev_prev_pt, *prev_pt, *next_pt) > min_cos){
                output.push_back(pt);
                is_discarded = false;
                continue;
            }
            else if(prev_pt && next_pt && next_next_pt && calcAngle(*prev_pt, *next_pt, * next_next_pt) > min_cos){
                output.push_back(pt);
                is_discarded = false;
                continue;
            }
            else{
                is_discarded = true;
                continue;
            }
        }

        output.push_back(pt);
        is_discarded = false;
    }
    output.push_back(input.back());
}

DottedCurve::DottedCurve()
  : d(new Data()) {
}

DottedCurve::~DottedCurve() {
    delete d;
}

void DottedCurve::setInputPoints(const std::vector<osg::Vec3>& pts) {
    d->pts = pts;
}

void DottedCurve::setInputPoints(std::vector<osg::Vec3>&& pts) {
    d->pts = pts;
}

void DottedCurve::setIsClosed(bool val) {
    d->is_closed = val;
}

void DottedCurve::sort() {
    
}

void DottedCurve::downsample(float max_dist, float max_angle, int max_iterations) {
    if (d->pts.size() < 3) return;

    std::vector<osg::Vec4> pts;
    pts.reserve(d->pts.size());

    for (auto& pt : d->pts) {
        pts.push_back(osg::Vec4(pt, 0));
    }

    std::vector<osg::Vec4> output;
    auto                   max_dist_squared = max_dist * max_dist;
    auto                   min_cos          = cos(max_angle);

    while (max_iterations > 0) {
        if (d->is_closed) {
            auto& pt_first = pts.front();
            auto& pt_last  = pts.back();
            pt_first.w()   = calcAngle(pt_first, pt_last, pts[1]);
            pt_last.w()    = calcAngle(pts[pts.size() - 2], pt_last, pt_first);
        }

        for (int i = 1; i < pts.size() - 1; i++) {
            auto& pt      = pts[i];
            auto& prev_pt = pts[i - 1];
            auto& next_pt = pts[i + 1];
            pt.w()        = calcAngle(prev_pt, pt, next_pt);
        }

        glv::downsample(pts, d->is_closed, min_cos, max_dist_squared, output);
        pts = output;
        --max_iterations;
    }

    d->pts.clear();

    for (auto& pt : output) {
        d->pts.push_back(osg::Vec3(pt.x(), pt.y(), pt.z()));
    }
}

std::vector<osg::Vec3> DottedCurve::getPoints() const {
    return d->pts;
}

osg::MatrixTransform* DottedCurve::createGeometry(const osg::Vec4& points_color,
                                                  const osg::Vec4& lines_color,
                                                  bool             points_visible,
                                                  bool             lines_visible) const {
    auto vertices = new osg::Vec3Array();
    auto colors   = new osg::Vec4Array();
    vertices->reserve(d->pts.size());
    for (auto& pt : d->pts) {
        vertices->push_back(pt);
    }
    auto geom = new osg::Geometry();
    geom->setVertexArray(vertices);
    geom->setColorArray(colors, osg::Array::BIND_PER_PRIMITIVE_SET);
    if (lines_visible) {
        colors->push_back(lines_color);
        geom->addPrimitiveSet(new osg::DrawArrays(d->is_closed ? GL_LINE_LOOP : GL_LINE_STRIP, 0, vertices->size()));
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

    return mt;
}
} // namespace glv