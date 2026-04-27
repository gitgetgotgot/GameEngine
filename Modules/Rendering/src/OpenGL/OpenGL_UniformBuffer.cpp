#include <Rendering/OpenGL/OpenGL_UniformBuffer.h>

OpenGL_UniformBuffer::OpenGL_UniformBuffer() {
	glGenBuffers(1, &id);
}

OpenGL_UniformBuffer::~OpenGL_UniformBuffer() {
	glDeleteBuffers(1, &id);
}

void OpenGL_UniformBuffer::setData(const void* data, size_t size) {
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
	glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void OpenGL_UniformBuffer::updateData(const void* data, size_t size, size_t offset) {
	glBindBuffer(GL_UNIFORM_BUFFER, id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void OpenGL_UniformBuffer::bind(uint32_t slot) {
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, id);
}
