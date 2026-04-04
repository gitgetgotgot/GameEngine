#pragma once
#include "Renderer.h"
#include "CanvasSystem.h"

constexpr uint32_t MAX_UI_ELEMENTS = 500;
constexpr uint32_t MAX_TEXT_SYMBOLS = 2000;

constexpr glm::vec3 UI_InstanceVertices[] = {
	glm::vec3(-0.5f, -0.5f, 0.0f),
	glm::vec3(-0.5f,  0.5f, 0.0f),
	glm::vec3(0.5f,  0.5f, 0.0f),
	glm::vec3(0.5f, -0.5f, 0.0f)
};
constexpr uint32_t UI_InstanceIndices[] = {
	0, 1, 2,
	0, 2, 3
};

class UI_RenderManager {
public:
	static UI_RenderManager* get_Instance() {
		static UI_RenderManager uiRenderSystem;
		return &uiRenderSystem;
	}
	void init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
	void update();
	void render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
private:
	UI_RenderManager() {}
	~UI_RenderManager() {}

	Engine::Systems::CanvasSystem* canvasSystem = nullptr;
	Engine::Systems::UI_TransformSystem* ui_transformSystem = nullptr;

	std::unique_ptr<Shader> shader;
	BasicShaderUBData basicUniformData{};
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<UniformBuffer> uniformBuffer;
	std::unique_ptr<StorageBuffer> storageBuffer;

	std::unique_ptr<Shader> text_shader;
	std::unique_ptr<StorageBuffer> storageTextBuffer;

	std::vector<Engine::Systems::UI_InstanceData> uiData;
	std::vector<Engine::Systems::UI_InstanceData> uiTextData;
};