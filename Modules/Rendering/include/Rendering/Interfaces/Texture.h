#pragma once
#include <cstdint>

class Texture {
public:
	~Texture() {}
	virtual void bind(uint32_t slot = 0) = 0;
	virtual void unbind() = 0;
	virtual uint64_t get_handle() = 0;
	uint32_t texture_id{};
};