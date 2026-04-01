#pragma once
#include "TransformSystem.h"
#include "SpriteComponentSystem.h"
#include "Renderer.h"
#include "Camera.h"

struct SpriteInstanceData {
	SpriteInstanceData() {}
	SpriteInstanceData(Sprite& sprite, glm::mat4& mat) {
		modelMatrix = mat;
		UV[0] = sprite.UV[0]; UV[1] = sprite.UV[1]; UV[2] = sprite.UV[2]; UV[3] = sprite.UV[3];
	}
	glm::mat4 modelMatrix{};
	glm::vec2 UV[4]{};
};

constexpr glm::vec3 spriteInstanceVertices[] = {
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f,  0.5f, 0.0f),
	glm::vec3( 0.5f,  0.5f, 0.0f),
	glm::vec3( 0.5f, -0.5f, 0.0f)
};
constexpr uint32_t spriteInstanceIndices[] = {
	0, 1, 2,
	0, 2, 3
};

class SpriteRenderManager {
public:
	static SpriteRenderManager* get_Instance() {
		static SpriteRenderManager spriteRenderSystem;
		return &spriteRenderSystem;
	}
	void init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
	void update(Engine::Component::Camera& activeCamera);
	void render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
private:
	Engine::Systems::TransformSystem* trSystem = nullptr;
	Engine::Systems::SpriteComponentSystem* srSystem = nullptr;

	std::unique_ptr<Shader> shader;
	BasicShaderUBData basicUniformData;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<UniformBuffer> uniformBuffer;
	std::unique_ptr<StorageBuffer> storageBuffer;

	sparse_set<SpriteInstanceData> spritesData;
	//std::unordered_map<std::uint32_t, sparse_set<SpriteInstanceData>> spriteInstanceData;
	SpriteRenderManager() {}
	~SpriteRenderManager() {}
};
