#include "ModelDefs.h"
#include <osgVerse/Pipeline/Pipeline.h>

namespace glv {
namespace {
osg::Texture2D* createDefaultTexture(const osg::Vec4& color) {
    osg::ref_ptr<osg::Image> image = new osg::Image;
    image->allocateImage(1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE);
    image->setInternalTextureFormat(GL_RGBA);

    osg::Vec4ub* ptr = (osg::Vec4ub*)image->data();
    *ptr             = osg::Vec4ub(color[0] * 255, color[1] * 255, color[2] * 255, color[3] * 255);

    osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
    tex2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::NEAREST);
    tex2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::NEAREST);
    tex2D->setWrap(osg::Texture2D::WRAP_S, osg::Texture2D::REPEAT);
    tex2D->setWrap(osg::Texture2D::WRAP_T, osg::Texture2D::REPEAT);
    tex2D->setImage(image.get());
    return tex2D.release();
}
} // namespace

void setPipelineMask(osg::Node* node, int mask) {
    osgVerse::Pipeline::setPipelineMask(*node, mask);
}

void applyDefaultMaterial(osg::Node* node) {
    auto ss = node->getOrCreateStateSet();
    ss->setTextureAttributeAndModes(0, createDefaultTexture(osg::Vec4(0.8f, 0.8f, 0.8f, 0.0f))); // DiffuseMap
    ss->setTextureAttributeAndModes(2, createDefaultTexture(osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f))); // SpecularMap
    ss->setTextureAttributeAndModes(3, createDefaultTexture(osg::Vec4(1.0f, 0.2f, 0.8f, 1.0f))); // ShininessMap
    ss->setTextureAttributeAndModes(4, createDefaultTexture(osg::Vec4(0.2f, 0.2f, 0.2f, 1.0f))); // AmbientMap
}
} // namespace glv