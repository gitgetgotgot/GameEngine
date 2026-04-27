#pragma once
#include <Rendering/Interfaces/IndirectBuffer.h>
#include "glad/glad.h"

class OpenGL_IndirectBuffer : public IndirectBuffer {
public:
    OpenGL_IndirectBuffer();
    ~OpenGL_IndirectBuffer();
    void setData(const void* data, size_t size) override;
    void updateData(const void* data, size_t size, size_t offset = 0) override;
    void bind() override;
private:
    GLuint id;
};