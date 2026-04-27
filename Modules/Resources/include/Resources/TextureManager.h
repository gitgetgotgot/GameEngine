#pragma once
#include <Utility/SparseSet.h>
#include <memory>
#include <Rendering/Interfaces/Renderer.h>

constexpr size_t MAX_TEXTURES = 2000;

class TextureManager {
public:
	static TextureManager* get_Instance() {
		static TextureManager texMgr;
		return &texMgr;
	}
	void init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
	Texture* create_texture_from_file(const char* filePath, bool isPixelised);
	Texture* create_texture_from_data(
		const unsigned char* image_bytes,
		int imgWidth,
		int imgHeight,
		int numOfColorChannels,
		bool isPixelised
	);
	void remove(uint32_t id);
	Texture* get(uint32_t id);
private:
	TextureManager() {}
	~TextureManager() {}
	sparse_set<std::unique_ptr<Texture>> textures;
	std::vector<uint32_t> free_ids;
	std::unique_ptr<StorageBuffer> textureHandlesBuffer;
	std::vector<uint64_t> textureHandles; //should also make as sparse_set probably
	Engine::Graphics::RendererInterface* renderer;
};