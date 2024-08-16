#include "SceneNodeGroup.h"

#include <vector>

#include "SceneNode.h"
#include "SceneNode_p.h"

namespace glr {
VI_OBJECT_META_IMPL(SceneNodeGroup, SceneNode);

struct SceneNodeGroup::Data {

    std::vector<vine::RefPtr<SceneNode>> child_nodes;
};

SceneNodeGroup::SceneNodeGroup()
  : d(new Data()) {
}

SceneNodeGroup::~SceneNodeGroup() {
    delete d;
}

void SceneNodeGroup::addChild(SceneNode* node) {
    if (!node) {
        return;
    }

    auto iter = std::find(d->child_nodes.begin(), d->child_nodes.end(), node);
    if (iter == d->child_nodes.end()) {
        d->child_nodes.push_back(node);
        node->d->addParent(this);
        dirtyBound();
    }
}

void SceneNodeGroup::removeChild(SceneNode* node) {
    auto iter = std::find(d->child_nodes.begin(), d->child_nodes.end(), node);
    if (iter != d->child_nodes.end()) {
        d->child_nodes.erase(iter);
        node->d->removeParent(this);
    }
}

int SceneNodeGroup::getNbChildren() const {
    return d->child_nodes.size();
}

SceneNode* SceneNodeGroup::getChildAt(int idx) const {
    return d->child_nodes.at(idx).get();
}

void SceneNodeGroup::onComputeBoundingBox(BoundingBox& bb) const {
    bb.set(0, 0, 0, 0, 0, 0);
    for (auto& child : d->child_nodes) {
        bb.combine(child->getBoundingBox());
    }
}


bool SceneNodeGroup::handleEvent(Event* e) {
    auto handled = EventReceiver::handleEvent(e);
    for(auto& child : d->child_nodes){
        handled |= child->handleEvent(e);
        if(handled)
        {
            break;
        }
    }
    return handled;
}

void SceneNodeGroup::update(UpdateContext* ctx){
    EventReceiver::update(ctx);
    for(auto& child : d->child_nodes){
        child->update(ctx);
    }
}
} // namespace glr