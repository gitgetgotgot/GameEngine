#pragma once
#include <cstdint>

class Texture {
public:
	~Texture() {}
	virtual void bind(uint32_t slot = 0) = 0;
	virtual void unbind() = 0;
};