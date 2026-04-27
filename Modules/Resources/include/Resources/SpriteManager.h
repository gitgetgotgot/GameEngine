#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

struct Sprite {
	Sprite() {}
	Sprite(uint32_t tex_id) {
		texture_id = tex_id;
	}
	Sprite(glm::vec2* uv, uint32_t tex_id) {
		texture_id = tex_id;
		UV[0] = uv[0]; UV[1] = uv[1]; UV[2] = uv[2]; UV[3] = uv[3];
	}
	glm::vec2 UV[4] = {
		{0.f, 0.f},
		{0.f, 1.f},
		{1.f, 1.f},
		{1.f ,0.f}
	};
	uint32_t texture_id = 0;
};

class SpriteManager {
public:
	static SpriteManager* get_Instance() {
		static SpriteManager spriteMgr;
		return &spriteMgr;
	}
	void add(uint32_t tex_id);
	void add_specified(glm::vec2* uv, uint32_t tex_id);
	void update(uint32_t sprite_id, glm::vec2* uv);
	void erase(uint32_t sprite_id);
	Sprite& get(uint32_t sprite_id);
private:
	SpriteManager() {}
	~SpriteManager() {}
	std::vector<Sprite> sprites;
};