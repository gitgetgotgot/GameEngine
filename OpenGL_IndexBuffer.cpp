#include "OpenGL_IndexBuffer.h"

OpenGL_IndexBuffer::OpenGL_IndexBuffer() {
	glGenBuffers(1, &id);
}

OpenGL_IndexBuffer::~OpenGL_IndexBuffer() {
	glDeleteBuffers(1, &id);
}

void OpenGL_IndexBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

void OpenGL_IndexBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGL_IndexBuffer::set_data(const void* data, size_t data_size, int usage) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, data, usage);
}

void OpenGL_IndexBuffer::update_data(const void* data, size_t data_size) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data_size, data);
}
