#pragma once
#include "Renderer.h"
#include "TransformSystem.h"
#include "MeshComponentSystem.h"
#include "MaterialManager.h"
#include "Camera.h"

constexpr uint32_t MAX_MESH_VERTEX_BUFFER_SIZE = 1'000'000;
constexpr uint32_t MAX_MESH_INDEX_BUFFER_SIZE = 2'000'000;
constexpr uint32_t MAX_SUBMESH_INSTANCES = 20000;
constexpr uint32_t MAX_INDIRECT_COMMANDS = 2000;

struct alignas(16) SubMeshInstanceData {
	SubMeshInstanceData() {}
	SubMeshInstanceData(glm::mat4& model, uint32_t& materialID)
		: modelMatrix{ model }, materialID{ materialID } {}
	glm::mat4 modelMatrix{ 1.0f }; // 64 bytes
	uint32_t materialID = 0;       // 4  bytes
	uint32_t padding[3]{ 0 };      // 12 bytes
};

class MeshRenderManager {
public:
	static MeshRenderManager* get_Instance() {
		static MeshRenderManager meshRenderSystem;
		return &meshRenderSystem;
	}
	void init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
	void add_indices_to_buffer(std::vector<uint32_t>& indices, uint32_t indexCount);
	void update(Engine::Component::Camera& activeCamera);
	void render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
	
	void load_submesh(
		std::vector<uint32_t>& submesh_indices,
		std::vector<SubMeshVertex>& submesh_vertices,
		SubMesh& submesh);
	void load_submeshes(
		std::vector<uint32_t>& submeshes_indices,
		std::vector<SubMeshVertex>& submeshes_vertices,
		std::vector<SubMesh>& submeshes);
	void update_indirect_base_instances(); //start index can be added to optimise a bit
private:
	SubMeshManager* subMeshMgr = nullptr;
	MeshManager* meshMgr = nullptr;
	MaterialManager* matMgr = nullptr;
	Engine::Systems::TransformSystem* trSystem = nullptr;
	Engine::Systems::MeshComponentSystem* mcSystem = nullptr;

	std::unique_ptr<Shader> shader;
	BasicShaderUBData basicUniformData;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<UniformBuffer> uniformBuffer;
	std::unique_ptr<StorageBuffer> storageBuffer;
	std::unique_ptr<IndirectBuffer> indirectBuffer;

	std::vector<SubMeshInstanceData> submeshesData;
	uint32_t index_current_global_size = 0;
	uint32_t vertex_current_global_size = 0;
	std::vector<DrawElementsIndirectCommand> drawCommands;
	std::vector<std::pair<uint32_t, bool>> submeshOffsets;
	bool submesh_inst_key = false;
	MeshRenderManager() {}
	~MeshRenderManager() {}
};