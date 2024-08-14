#include "SkyBox.h"

#include <vine/core/Ptr.h>

#include "Callbacks.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Depth.h"
#include "Geometry.h"
#include "Model.h"
#include "Renderer.h"
#include "Shader.h"
#include "State.h"
#include "StateSet.h"
#include "Uniform.h"

namespace glr {
namespace {
const char* sky_box_vs = R"(
#version 330 core
layout(location=0) in vec3 position;
uniform mat4 matrix_mvp_;
out vec3 frag_tex_coord;
void main(){
    vec4 posi = matrix_mvp_ * vec4(position, 1.0);
    posi.z = posi.w;
    gl_Position = posi;
    frag_tex_coord = position;
}
        )";
const char* sky_box_fs = R"(
#version 330 core
#extension GL_ARB_shader_image_load_store : enable
layout(early_fragment_tests) in;
uniform samplerCube tex;
in vec3 frag_tex_coord;
out vec4 FragColor;
void main(){
    vec4 color = texture(tex, frag_tex_coord);
    FragColor = color;
}
        )";

struct SkyBoxUpdateCallback : public UpdateCallback {
    SkyBoxUpdateCallback(Uniform* uniform) { uniform_ = uniform; }
    virtual void operator()(Object* obj, UpdateContext* ctx) override {
        auto model    = obj->cast<Model>();
        auto stateset = model->getOrCreateStateSet();
        auto shader   = stateset->getShader();
        auto cam      = ctx->getCurrentRenderer()->getCamera();

        auto mat_v = cam->getViewMatrix();
        mat_v[3]   = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::mat4 m1(1.);
        m1         = glm::rotate(m1, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        auto mat_p = cam->getProjectionMatrix();

        uniform_->setMat4x4(mat_p * mat_v * m1);
    }

    Uniform* uniform_ = nullptr;
};
} // namespace

Model* createSkyBox(CubeMap* tex) {
    auto cube = Geometry::createCube(1, 0, -1, -1, -1);
    cube->addTexture(0, "tex", tex);
    auto shader = new Shader(sky_box_vs, {}, sky_box_fs);
    auto model  = new Model();

    auto uniform = new Uniform("matrix_mvp_", glm::mat4x4());

    model->addDrawable(cube);
    model->addUpdateCallback(new SkyBoxUpdateCallback(uniform));
    model->getOrCreateStateSet()->setShader(shader);
    model->getOrCreateStateSet()->setAttribute(uniform);
    model->getOrCreateStateSet()->setAttribute(new Depth(1, 1, Depth::LEQUAL, true));
    return model;
}
} // namespace glr