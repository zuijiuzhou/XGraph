#pragma once

namespace osg {
class Node;
}

namespace glv {

enum PipelineMask
{
    DEFERRED_SCENE_MASK = 0xff000000,
    FORWARD_SCENE_MASK  = 0x000000ff,
    USER_INPUT_MASK     = 0x00001000,
    SHADOW_CASTER_MASK  = 0x00100000
};

void setPipelineMask(osg::Node* node, int mask);

void applyDefaultMaterial(osg::Node* node);
} // namespace glv