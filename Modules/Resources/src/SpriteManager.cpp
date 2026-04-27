#include <Resources/SpriteManager.h>

void SpriteManager::add(uint32_t tex_id) {
	sprites.emplace_back(Sprite(tex_id));
}

void SpriteManager::add_specified(glm::vec2* uv, uint32_t tex_id) {
	sprites.emplace_back(Sprite(uv, tex_id));
}

void SpriteManager::update(uint32_t sprite_id, glm::vec2* uv) {
	Sprite& sprite = sprites[sprite_id];
	sprite = Sprite(uv, sprite.texture_id);
}

void SpriteManager::erase(uint32_t sprite_id) {
	sprites.erase(sprites.begin() + sprite_id);
}

Sprite& SpriteManager::get(uint32_t sprite_id) {
	return sprites[sprite_id];
}