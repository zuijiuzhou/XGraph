#include "SceneNode_p.h"

namespace glr {

void SceneNodePrivate::addParent(SceneNode* node) {
    auto iter = std::find(parent_nodes.begin(), parent_nodes.end(), node);
    if (iter == parent_nodes.end()) {
        parent_nodes.push_back(node);
    }
}

void SceneNodePrivate::removeParent(SceneNode* node) {
    auto iter = std::find(parent_nodes.begin(), parent_nodes.end(), node);
    if (iter != parent_nodes.end()) {
        parent_nodes.erase(iter);
    }
}
} // namespace glr