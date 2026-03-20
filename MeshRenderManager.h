#pragma once
#include "Renderer.h"
#include "TransformSystem.h"
#include "MeshRendererSystem.h"
#include "Camera.h"

class MeshRenderManager {
public:
	static MeshRenderManager* get_Instance() {
		static MeshRenderManager meshRenderSystem;
		return &meshRenderSystem;
	}
	void init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
	void load_basic_meshes(std::vector<uint32_t>& indices, std::vector<MeshVertex>& vertices);
	void add_indices_to_buffer(std::vector<uint32_t>& indices, uint32_t indexCount);
	void update(Engine::Component::Camera& activeCamera);
	void render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
private:
	TransformSystem* trSystem;
	MeshRendererSystem* mrSystem;

	std::unique_ptr<Shader> shader;
	BasicShaderUBData basicUniformData;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<UniformBuffer> uniformBuffer;
	std::unique_ptr<StorageBuffer> storageBuffer;
	std::unique_ptr<IndirectBuffer> indirectBuffer;

	sparse_set<glm::mat4> meshModelMatrices;
	std::vector<DrawElementsIndirectCommand> drawCommands;
	MeshRenderManager() {}
	~MeshRenderManager() {}
};