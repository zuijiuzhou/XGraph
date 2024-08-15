#pragma once

#include "glrenderer_global.h"

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "StateAttribute.h"

namespace glr {
class GraphicContext;
class GLRENDERER_API Light : public Object {
    VI_OBJECT_META;

  public:
    Light();

  public:
    glm::vec4 getAmbient() const;
    void      setAmbient(const glm::vec4& color);

    glm::vec4 getDiffuse() const;
    void      setDiffuse(const glm::vec4& color);

    glm::vec4 getSpecular() const;
    void      setSpecular(const glm::vec4& color);

    glm::vec3 getDirection() const;
    void      setDirection(const glm::vec3& dir);

    glm::vec4 getPosition() const;
    void      setPosition(const glm::vec4& pos);

    float getConstantAttenuation() const;
    void  setConstantAttenuation(float val);

    float getLinearAttenuation() const;
    void  setLinearAttenuation(float val);

    float getQuadraticAttenuation() const;
    void  settQuadraticAttenuation(float val);

    float getCutoff() const;
    void  setCutoff(float val);

    float getExponent() const;
    void  setExponent(float val);

    void setIsHead(bool head);
    bool getIsHead() const;

  private:
    glm::vec4 a_, d_, s_;
    glm::vec4 pos_;
    glm::vec3 dir_;
    float     k_c_, k_l_, k_q_;
    float     expo_, co_;
    bool      head_ = false;
};

class GLRENDERER_API Lights : public StateAttribute {
  public:
    void addLight(Light* l);

    void removeLight(Light* l);

    std::vector<Light*> getLights() const;

    virtual Type getType() const override;

  protected:
    virtual void apply(State& state) const override;

  public:
    static int getMaxLight();

  private:
    std::vector<Light*> lights_;
};
} // namespace glr