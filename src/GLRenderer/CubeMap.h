#pragma once
#include "Texture.h"
#include <string>
#include <vector>

namespace glr {
class Image;
class CubeMap : public Texture {
    VI_OBJECT_META;

  public:
    CubeMap();
    virtual ~CubeMap();

  public:
    virtual Type getType() const override;
    void         setImages(const std::vector<std::string>& imgs);
    void         setImages(const std::vector<Image*>& imgs);

  protected:
    virtual GLuint onCreate(State& ctx) override;
    virtual bool   onUpdate(State& ctx) override;

  private:
    VI_OBJECT_DATA;
};
} // namespace glr