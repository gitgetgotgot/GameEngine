#include <Rendering/OpenGL/OpenGL_IndirectBuffer.h>

OpenGL_IndirectBuffer::OpenGL_IndirectBuffer() {
	glGenBuffers(1, &id);
}

OpenGL_IndirectBuffer::~OpenGL_IndirectBuffer() {
	glDeleteBuffers(1, &id);
}

void OpenGL_IndirectBuffer::setData(const void* data, size_t size) {
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, id);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void OpenGL_IndirectBuffer::updateData(const void* data, size_t size, size_t offset) {
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, id);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, size, data);
}

void OpenGL_IndirectBuffer::bind() {
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, id);
}
