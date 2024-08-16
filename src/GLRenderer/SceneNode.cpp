#include "SceneNode.h"

#include <vector>

#include "SceneNode_p.h"
#include "StateSet.h"

namespace glr {

VI_OBJECT_META_IMPL(SceneNode, EventReceiver);

SceneNode::SceneNode()
  : d(new SceneNodePrivate()) {
}

SceneNode::~SceneNode() {
    delete d;
}

glm::mat4 SceneNode::getMatrix() const {
    return d->matrix;
}

void SceneNode::setMatrix(const glm::mat4& mat) {
    d->matrix = mat;
}

StateSet* SceneNode::getOrCreateStateSet() {
    if (!d->state_set.get()) {
        d->state_set = new StateSet();
    }
    return d->state_set.get();
}

StateSet* SceneNode::getStateSet() {
    return d->state_set.get();
}

BoundingBox SceneNode::getBoundingBox() const {
    if (d->bound_is_dirty) {
        onComputeBoundingBox(d->bb);
    }
    return d->bb;
}

void SceneNode::setBoundingBox(const BoundingBox& bb){
    d->bb = bb;
    d->bound_is_dirty = false;
}

int SceneNode::getNbParents() const {
    return d->parent_nodes.size();
}

SceneNode* SceneNode::getParentAt(int idx) const {
    return d->parent_nodes.at(idx);
}

void SceneNode::dirtyBound() {
    d->bound_is_dirty = true;
    int nparents = getNbParents();
    for(int i = 0; i < nparents; ++i){
        getParentAt(i)->dirtyBound();
    }
}

void SceneNode::onComputeBoundingBox(BoundingBox& bb) const {
    bb.set(0, 0, 0, 0, 0, 0);
}

} // namespace glr