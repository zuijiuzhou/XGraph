#include "SkyBox.h"

#include <vine/core/Ptr.h>

#include "Camera.h"
#include "CubeMap.h"
#include "Depth.h"
#include "Geometry.h"
#include "Model.h"
#include "Shader.h"
#include "State.h"
#include "StateSet.h"

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

struct SkyBoxUpdateCallback : public ModelCallback {
    SkyBoxUpdateCallback()
      : ModelCallback(UPDATE) {}
    virtual void operator()(State& state, Model* model) override {
        auto shader = state.getCurrentShader();
        auto cam    = state.getCurrentCamera();
        auto mat_v  = cam->getViewMatrix();
        mat_v[3]    = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::mat4 m1(1.);
        m1         = glm::rotate(m1, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
        auto mat_p = cam->getProjectionMatrix();

        shader->set(state, "matrix_mvp_", mat_p * mat_v * m1);
    }
};
} // namespace

Model* createSkyBox(CubeMap* tex) {
    auto cube = Geometry::createCube(1, 0, -1, -1, -1);
    cube->addTexture(0, "tex", tex);
    auto shader = new Shader(sky_box_vs, {}, sky_box_fs);
    auto model  = new Model();
    model->addDrawable(cube);
    model->addCallback(new SkyBoxUpdateCallback());
    model->getOrCreateStateSet()->setShader(shader);
    model->getOrCreateStateSet()->setAttribute(new Depth(1, 1, Depth::LEQUAL, true));
    return model;
}
} // namespace glr