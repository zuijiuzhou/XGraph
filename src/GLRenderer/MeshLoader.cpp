#include "MeshLoader.h"
#include "Geometry.h"
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <filesystem>

namespace glr {

namespace ai = Assimp;
namespace {
glm::vec4 s_default_face_color = glm::vec4(0.88, 0.88, 0.88, 1.0);

void aiProcessNode(Model* group, const aiScene* scene, aiNode* node) {
    for (int i = 0; i < node->mNumMeshes; i++) {
        auto        mesh = scene->mMeshes[node->mMeshes[i]];
        auto        geom = new Geometry();
        BoundingBox bb;
        if (mesh->HasPositions()) {
            auto vertices = new Vec3fArray();
            vertices->reserve(mesh->mNumVertices);
            glm::vec4 prev_vertex_color = s_default_face_color;
            for (int j = 0; j < mesh->mNumVertices; j++) {
                vertices->push_back(glm::vec3(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z));
                bb.expandBy(vertices->back());
            }
            geom->addVertexAttribArray(0, vertices);
        }
        if (mesh->HasVertexColors(0)) {
            auto colors = new Vec4fArray();
            colors->reserve(mesh->mNumVertices);
            auto data = mesh->mColors[0];
            for (int j = 0; j < mesh->mNumVertices; j++) {
                colors->push_back(glm::vec4(data[j].r, data[j].g, data[j].b, data[j].a));
            }
            geom->addVertexAttribArray(2, colors);
        }
        else {
            auto colors = new Vec4fArray();
            colors->push_back(s_default_face_color);
            geom->addVertexAttribArray(2, colors);
        }
        if (mesh->HasTextureCoords(0)) {
            // todo
        }
        if (mesh->HasNormals()) {
            auto norms = new Vec3fArray();
            norms->reserve(mesh->mNumVertices);
            for (int j = 0; j < mesh->mNumVertices; j++) {
                norms->push_back(glm::vec3(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z));
            }
            geom->addVertexAttribArray(1, norms);
        }

        if (mesh->HasFaces()) {
            auto indices = std::vector<GLuint>();
            indices.reserve(mesh->mNumFaces * 3);
            for (auto j = 0; j < mesh->mNumFaces; j++) {
                auto face = mesh->mFaces[j];
                if (face.mNumIndices == 3) {
                    indices.push_back(face.mIndices[0]);
                    indices.push_back(face.mIndices[1]);
                    indices.push_back(face.mIndices[2]);
                }
            }
            auto prim = new DrawElementsUInt(PrimitiveSet::MODE_TRIANGLES);
            prim->setIndices(indices);
            geom->addPrimitiveSet(prim);
        }
        geom->setBoundingBox(bb);
        group->addDrawable(geom);
    }
    for (int i = 0; i < node->mNumChildren; i++) {
        auto child_node = node->mChildren[i];
        aiProcessNode(group, scene, child_node);
    }
}
} // namespace

bool MeshLoader::isSupported(const std::string& file) {
    namespace fs = std::filesystem;
    fs::path path(file);
    if (!path.has_extension()) return false;
    auto                         file_ext = path.extension().string();
    static std::set<std::string> exts     = { ".stl", ".obj", ".gltf", ".3mf", ".3ds", ".ply", ".dxf",
                                              ".ifc", ".ac",  ".ac3d", ".lxo", ".fbx", ".dae" };
    return exts.contains(file_ext);
}

Model* MeshLoader::loadFile(const std::string& file) {
    auto         root = new Model();
    ai::Importer im;
    auto scene = im.ReadFile(file, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_GenSmoothNormals);
    if (scene) {
        aiProcessNode(root, scene, scene->mRootNode);
    }
    return root;
}
} // namespace glr