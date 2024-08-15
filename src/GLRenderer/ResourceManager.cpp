#include "ResourceManager.h"

#include <map>

#include <vine/core/Ptr.h>

#include "StdUtils/Resources.h"

#include "CubeMap.h"
#include "Shader.h"

namespace glr {

namespace {
std::map<std::string, vine::RefPtr<Shader>> s_shaders;
}

ResourceManager::ResourceManager() {
}

ResourceManager* ResourceManager::instance() {
    static ResourceManager* inst = new ResourceManager();
    return inst;
}

Shader* ResourceManager::getShader(const std::string& name) {
    if (s_shaders.find(name) == s_shaders.end()) {
        auto vs = "shaders/" + name + ".vs.glsl";
        auto gs = "shaders/" + name + ".gs.glsl";
        auto fs = "shaders/" + name + ".fs.glsl";

        auto shader = Shader::create(__RES("") + vs, __RES("") + gs, __RES("") + fs);
        if (shader) {
            s_shaders.insert({ name, shader });
        }
        return shader;
    }
    return s_shaders[name].get();
}

Shader* ResourceManager::getInternalShader(InternalShader shader) {
    if (shader == IS_Base) {
        return getShader("Base");
    }
    else if (shader == IS_Geometry) {
        return getShader("MeshShape");
    }
    else if (shader == IS_PointCloud) {
        return getShader("PointCloud");
    }
    else if (shader == IS_SkyBox) {
        return getShader("SkyBox");
    }
    return nullptr;
}

CubeMap* ResourceManager::getInternalCubeMap(InternalCubeMap map) {
    std::vector<std::string> files;
    if (map == ICM_CubeMap1) {
        files.push_back(__RES("images/right.jpg"));
        files.push_back(__RES("images/left.jpg"));
        files.push_back(__RES("images/top.jpg"));
        files.push_back(__RES("images/bottom.jpg"));
        files.push_back(__RES("images/front.jpg"));
        files.push_back(__RES("images/back.jpg"));
    }
    else if (map == ICM_CubeMap2) {
        files.push_back(__RES("images/posx.jpg"));
        files.push_back(__RES("images/negx.jpg"));
        files.push_back(__RES("images/posy.jpg"));
        files.push_back(__RES("images/negy.jpg"));
        files.push_back(__RES("images/posz.jpg"));
        files.push_back(__RES("images/negz.jpg"));
    }
    else
        return nullptr;
    auto cm = new CubeMap();
    cm->setImages(files);
    return cm;
}
} // namespace glr