#pragma once
#include "Renderer.h"
#include "TransformSystem.h"
#include "MeshComponentSystem.h"
#include "MaterialManager.h"
#include "Camera.h"

constexpr uint32_t MAX_MESH_VERTEX_BUFFER_SIZE = 1'000'000;
constexpr uint32_t MAX_MESH_INDEX_BUFFER_SIZE = 2'000'000;
constexpr uint32_t MAX_MESH_INSTANCES = 2000;
constexpr uint32_t MAX_INDIRECT_COMMANDS = 2000;

struct alignas(16) SubMeshInstanceData {
	SubMeshInstanceData() {}
	SubMeshInstanceData(
		glm::mat4& model,
		uint32_t& albedoID,
		uint32_t& normalID,
		uint32_t& roughnessID,
		uint32_t& metallicID
	) : modelMatrix{ model },
		albedoID { albedoID },
		normalID{ normalID },
		roughnessID{ roughnessID },
		metallicID{ metallicID } {}
	glm::mat4 modelMatrix {}; // 64 bytes
	uint32_t albedoID {};     // 4  bytes
	uint32_t normalID {};     // 4  bytes
	uint32_t roughnessID {};  // 4  bytes
	uint32_t metallicID {};   // 4  bytes
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
	Engine::Systems::MeshComponentSystem* mrSystem = nullptr;

	std::unique_ptr<Shader> shader;
	BasicShaderUBData basicUniformData;
	std::unique_ptr<VertexArray> vertexArray;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<UniformBuffer> uniformBuffer;
	std::unique_ptr<StorageBuffer> storageBuffer;
	std::unique_ptr<IndirectBuffer> indirectBuffer;

	sparse_set<SubMeshInstanceData> meshesInstanceData;
	uint32_t index_current_global_size = 0;
	uint32_t vertex_current_global_size = 0;
	std::vector<DrawElementsIndirectCommand> drawCommands;
	MeshRenderManager() {}
	~MeshRenderManager() {}
};