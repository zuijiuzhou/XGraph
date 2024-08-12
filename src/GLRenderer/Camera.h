#pragma once
#include "Object.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace glr {
class Camera : public Object {
    VI_OBJECT_META;

  public:
    Camera();

  public:
    glm::vec4 getClearColor() const;
    void      setClearColor(const glm::vec4& color);

    GLdouble getClearDepth() const;
    void     setClearDepth(GLdouble depth);

    GLint getClearStencil() const;
    void  setClearStencil(GLint val);

    GLbitfield getClearMask() const;
    void       setClearMask(GLbitfield mask);

    void getViewMatrixAsLookAt(glm::vec3& o_posi, glm::vec3& o_target, glm::vec3& o_up);
    void setViewMatrixAsLookAt(const glm::vec3& posi, const glm::vec3& target, const glm::vec3& up);

    void      setViewMatrix(const glm::mat4& mat);
    glm::mat4 getViewMatrix() const;

    glm::vec3 getViewDir() const;
    glm::vec3 getViewPos() const;

    void      setProjectionMatrix(const glm::mat4& mat);
    glm::mat4 getProjectionMatrix() const;

    glm::mat4 getViewProjectionMatrix() const;

    void setViewport(int x, int y, int w, int h);
    void getViewport(int& x, int& y, int& w, int& h) const;

    void apply() const;
    void applyViewport() const;
    void applyAllExceptViewport() const;

  private:
    glm::mat4  view_matrix_;
    glm::mat4  proj_matrix_;
    GLdouble   clear_depth_;
    GLint      clear_stencil_;
    glm::vec4  clear_color_;
    GLbitfield clear_mask_;
    double     vp_x_, vp_y_, vp_w_, vp_h_;
};
} // namespace glr