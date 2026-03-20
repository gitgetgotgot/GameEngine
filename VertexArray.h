#pragma once
#include "VertexBuffer.h"
#include <memory>


struct VertexAttribute {
	VertexAttribute() {}
	VertexAttribute(uint32_t index, uint32_t size, uint32_t offset, uint32_t stride)
		: index{ index }, size{ size }, offset{ offset }, stride{ stride } {}
	uint32_t index{}, size{}, offset{}, stride{};
};

class VertexArray {
public:
	virtual void bind() = 0;
	virtual void unbind() = 0;
	virtual void add_attribute(VertexAttribute attrib, std::unique_ptr<VertexBuffer>& vb) = 0;
	~VertexArray() {}
};