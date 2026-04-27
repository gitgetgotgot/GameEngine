#include <Resources/TextureManager.h>

void TextureManager::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	this->renderer = renderer.get();
	textureHandlesBuffer = renderer->createStorageBuffer();
	textureHandlesBuffer->bind(3);
	textureHandlesBuffer->setData(nullptr, MAX_TEXTURES * sizeof(uint64_t));
}

Texture* TextureManager::create_texture_from_file(const char* filePath, bool isPixelised) {
	uint32_t id;
	if (free_ids.size()) {
		id = free_ids.back();
		free_ids.pop_back();
	}
	else id = textures.size();
	Texture* tex = textures.add(renderer->createTextureFromFile(filePath, isPixelised), id)->get();
	tex->texture_id = id;

	//add this texture handle to storage buffer
	textureHandles.emplace_back(tex->get_handle());
	textureHandlesBuffer->updateData(textureHandles.data(), textureHandles.size() * sizeof(uint64_t));

	return tex;
}

Texture* TextureManager::create_texture_from_data(
	const unsigned char* image_bytes,
	int imgWidth,
	int imgHeight,
	int numOfColorChannels,
	bool isPixelised
) {
	uint32_t id;
	if (free_ids.size()) {
		id = free_ids.back();
		free_ids.pop_back();
	}
	else id = textures.size();
	Texture* tex = textures.add(renderer->createTextureFromData(image_bytes, imgWidth, imgHeight, numOfColorChannels, isPixelised), id)->get();
	tex->texture_id = id;

	//add this texture handle to storage buffer
	textureHandles.emplace_back(tex->get_handle());
	textureHandlesBuffer->updateData(textureHandles.data(), textureHandles.size() * sizeof(uint64_t));

	return tex;
}

void TextureManager::remove(uint32_t id) {
	textures.remove(id);
	free_ids.emplace_back(id);
}

Texture* TextureManager::get(uint32_t id) {
	return textures.get(id)->get();
}
