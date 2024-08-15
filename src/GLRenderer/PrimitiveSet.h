#pragma once

#include "glrenderer_global.h"

#include <glad/glad.h>

#include <vector>

#include "Array.h"

namespace glr {
class GLRENDERER_API PrimitiveSet : public Object {
    VI_OBJECT_META;

  public:
    enum Mode
    {
        MODE_POINTS        = GL_POINTS,
        MODE_LINES         = GL_LINES,
        MODE_LINE_LOOP     = GL_LINE_LOOP,
        MODE_LINE_STRIP    = GL_LINE_STRIP,
        MODE_TRIANGLES     = GL_TRIANGLES,
        MODE_TRIANGLE_TRIP = GL_TRIANGLE_STRIP,
        MODE_TRIANGLE_FAN  = GL_TRIANGLE_FAN,
        // no support in gl 3
        MODE_QUADS         = GL_QUADS
    };

  public:
    PrimitiveSet();
    PrimitiveSet(Mode mode, GLint insts);

  public:
    Mode getMode() const;
    void setMode(Mode mode);

    GLint getInstances() const;
    void  setInstances(GLint insts);

  private:
    Mode  mode_      = MODE_TRIANGLES;
    GLint instances_ = 1;

  public:
    virtual void draw() = 0;
};

class GLRENDERER_API DrawArrays : public PrimitiveSet {
    VI_OBJECT_META;

  public:
    DrawArrays();
    DrawArrays(Mode mode, GLint offset, GLsizei count);
    DrawArrays(Mode mode, GLint offset, GLsizei count, GLint insts);

  public:
    GLint getOffset() const;
    void  setOffset(GLint offset);

    GLsizei getCount() const;
    void    setCount(GLsizei count);

    virtual void draw() override;

  private:
    GLint   offset_ = 0;
    GLsizei count_  = 0;
};

class GLRENDERER_API DrawElementsUInt : public PrimitiveSet {
    VI_OBJECT_META;

  public:
    DrawElementsUInt();
    DrawElementsUInt(Mode mode);
    DrawElementsUInt(Mode mode, GLint insts);

  public:
    void                 setIndices(std::vector<GLuint> indices);
    std::vector<GLuint>& getIndices();

    virtual void draw() override;

  private:
    std::vector<GLuint> indices_;
};
}; // namespace glr