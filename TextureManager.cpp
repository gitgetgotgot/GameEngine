#include "TextureManager.h"

void TextureManager::load(const char* filePath, bool isPixelised, std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	textures.emplace_back(renderer->createTextureFromFile(filePath, isPixelised));
}

std::unique_ptr<Texture>& TextureManager::get(uint32_t id) {
	return textures[id];
}
