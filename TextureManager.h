#pragma once
#include <vector>
#include <memory>
#include "Renderer.h"

class TextureManager {
public:
	static TextureManager* get_Instance() {
		static TextureManager texMgr;
		return &texMgr;
	}
	void load(const char* filePath, bool isPixelised, std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
	std::unique_ptr<Texture>& get(uint32_t id);
private:
	TextureManager() {}
	~TextureManager() {}
	std::vector<std::unique_ptr<Texture>> textures;
};