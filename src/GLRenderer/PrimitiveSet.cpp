#include "PrimitiveSet.h"

namespace glr {
using Mode = PrimitiveSet::Mode;

VI_OBJECT_META_IMPL(PrimitiveSet, Object);
VI_OBJECT_META_IMPL(DrawArrays, PrimitiveSet);
VI_OBJECT_META_IMPL(DrawElementsUInt, PrimitiveSet);

PrimitiveSet::PrimitiveSet() {
}

PrimitiveSet::PrimitiveSet(Mode mode, GLint insts)
  : mode_(mode)
  , instances_(insts) {
}

Mode PrimitiveSet::getMode() const {
    return mode_;
}

void PrimitiveSet::setMode(Mode mode) {
    mode_ = mode;
}

GLint PrimitiveSet::getInstances() const {
    return instances_;
}

void PrimitiveSet::setInstances(GLint insts) {
    instances_ = insts;
}

DrawArrays::DrawArrays() {
}

DrawArrays::DrawArrays(Mode mode, GLint offset, GLsizei count)
  : DrawArrays(mode, offset, count, 1) {
}

DrawArrays::DrawArrays(Mode mode, GLint offset, GLsizei count, GLint insts)
  : PrimitiveSet(mode, insts)
  , offset_(offset)
  , count_(count) {
}

GLint DrawArrays::getOffset() const {
    return offset_;
}

void DrawArrays::setOffset(GLint offset) {
    offset_ = offset;
}

GLsizei DrawArrays::getCount() const {
    return count_;
}

void DrawArrays::setCount(GLsizei count) {
    count = count;
}

void DrawArrays::draw() {
    auto mode  = getMode();
    auto insts = getInstances();

    if (insts > 1) {
        glDrawArraysInstanced(mode, offset_, count_, insts);
    }
    else {
        glDrawArrays(mode, offset_, count_);
    }
}

DrawElementsUInt::DrawElementsUInt() {
}

DrawElementsUInt::DrawElementsUInt(Mode mode)
  : DrawElementsUInt(mode, 1) {
}

DrawElementsUInt::DrawElementsUInt(Mode mode, GLint insts)
  : PrimitiveSet(mode, insts) {
}

void DrawElementsUInt::setIndices(std::vector<GLuint> indices) {
    indices_ = std::move(indices);
}

std::vector<GLuint>& DrawElementsUInt::getIndices() {
    return indices_;
}

void DrawElementsUInt::draw() {
    if (indices_.empty()) return;
    auto mode  = getMode();
    auto insts = getInstances();
    if (insts > 1) {
        glDrawElementsInstanced(mode, indices_.size(), GL_UNSIGNED_INT, (void*)indices_.data(), insts);
    }
    else {
        glDrawElements(mode, indices_.size(), GL_UNSIGNED_INT, (void*)indices_.data());
    }
}
} // namespace glr