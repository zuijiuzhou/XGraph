#pragma once

#include "glrenderer_global.h"

#include <string>

#include "Texture.h"

namespace glr {
class Image;

class GLRENDERER_API Texture2D : public Texture {
    VI_OBJECT_META;

  public:
    Texture2D();
    virtual ~Texture2D();

  public:
    virtual Type getType() const override;

    void setWidth(GLsizei w);

    void setHeight(GLsizei h);

    GLsizei getWidth() const;

    GLsizei getHeight() const;

    void setImage(const std::string& img);

    void setImage(Image* image);

    bool save(const std::string& path) const;

  protected:
    virtual GLuint onCreate(State& state) override;

    virtual bool onUpdate(State& state) override;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr