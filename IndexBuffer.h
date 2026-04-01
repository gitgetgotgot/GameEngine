#pragma once

class IndexBuffer {
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void set_data(const void* data, size_t data_size, int usage) = 0;
	virtual void update_data(const void* data, size_t data_size, size_t offset = 0) = 0;
	~IndexBuffer() {}
};