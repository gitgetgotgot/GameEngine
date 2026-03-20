#include "OpenGL_StorageBuffer.h"

OpenGL_StorageBuffer::OpenGL_StorageBuffer() {
	glGenBuffers(1, &this->id);
}

OpenGL_StorageBuffer::~OpenGL_StorageBuffer() {
	glDeleteBuffers(1, &this->id);
}

void OpenGL_StorageBuffer::setData(const void* data, size_t size) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->id);
	glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void OpenGL_StorageBuffer::updateData(const void* data, size_t size) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->id);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, data);
}

void OpenGL_StorageBuffer::bind(uint32_t slot) {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slot, this->id);
}
