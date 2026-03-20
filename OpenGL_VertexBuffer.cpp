#include "OpenGL_VertexBuffer.h"

OpenGL_VertexBuffer::OpenGL_VertexBuffer() {
	glGenBuffers(1, &id);
}

OpenGL_VertexBuffer::~OpenGL_VertexBuffer() {
	glDeleteBuffers(1, &id);
}

void OpenGL_VertexBuffer::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void OpenGL_VertexBuffer::unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGL_VertexBuffer::set_data(const void* data, size_t data_size, int usage) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, data_size, data, usage);
}

void OpenGL_VertexBuffer::update_data(const void* data, size_t data_size) {
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, data_size, data);
}
