#include "MeshRenderManager.h"

void MeshRenderManager::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	ShaderDesc desc("", "Shaders/OpenGL/Mesh Vert.glsl", "Shaders/OpenGL/Mesh Frag.glsl");
	shader = renderer->createShader(desc);

	vertexArray = renderer->createVertexArray();
	vertexArray->bind();

	vertexBuffer = renderer->createVertexBuffer();
	vertexBuffer->set_data(nullptr, sizeof(SubMeshVertex) * MAX_MESH_VERTEX_BUFFER_SIZE, renderer->DYNAMIC_USAGE);

	vertexArray->add_attribute(VertexAttribute(0, 3, offsetof(SubMeshVertex, SubMeshVertex::pos), sizeof(SubMeshVertex)), vertexBuffer);
	vertexArray->add_attribute(VertexAttribute(1, 3, offsetof(SubMeshVertex, SubMeshVertex::normal), sizeof(SubMeshVertex)), vertexBuffer);
	vertexArray->add_attribute(VertexAttribute(2, 2, offsetof(SubMeshVertex, SubMeshVertex::UV), sizeof(SubMeshVertex)), vertexBuffer);

	indexBuffer = renderer->createIndexBuffer();
	indexBuffer->set_data(nullptr, sizeof(uint32_t) * MAX_MESH_INDEX_BUFFER_SIZE, renderer->DYNAMIC_USAGE);

	vertexArray->unbind();
	indexBuffer->unbind();

	uniformBuffer = renderer->createUniformBuffer();
	uniformBuffer->bind(1);
	uniformBuffer->setData(&basicUniformData, sizeof(BasicShaderUBData));

	storageBuffer = renderer->createStorageBuffer();
	storageBuffer->bind(1);
	storageBuffer->setData(nullptr, sizeof(SubMeshInstanceData) * MAX_SUBMESH_INSTANCES);

	submeshesData.resize(MAX_SUBMESH_INSTANCES);

	indirectBuffer = renderer->createIndirectBuffer();
	indirectBuffer->setData(nullptr, sizeof(DrawElementsIndirectCommand) * MAX_INDIRECT_COMMANDS);
	drawCommands.reserve(MAX_INDIRECT_COMMANDS);  //just reserve for commands
	submeshOffsets.resize(MAX_INDIRECT_COMMANDS, {0, false}); //resize to init empty

	trSystem = static_cast<Engine::Systems::TransformSystem*>(Engine::Systems::ComponentSystemsCore::get_Instance()->get_system<Engine::Component::Transform>());
	mcSystem = static_cast<Engine::Systems::MeshComponentSystem*>(Engine::Systems::ComponentSystemsCore::get_Instance()->get_system<Engine::Component::MeshComponent>());
	meshMgr = MeshManager::get_Instance();
	subMeshMgr = SubMeshManager::get_Instance();
	matMgr = MaterialManager::get_Instance();
}

void MeshRenderManager::add_indices_to_buffer(std::vector<uint32_t>& indices, uint32_t indexCount) {
	std::vector<uint32_t> addedIndices;
	addedIndices.resize(indexCount);

	uint32_t faces = indexCount / 6;
	for (int i = 0; i < faces; i++) {
		size_t face_index = i * 6;
		addedIndices[face_index] = addedIndices[face_index + 3] = i * 4;
		addedIndices[face_index + 2] = addedIndices[face_index + 4] = addedIndices[face_index] + 2;
		addedIndices[face_index + 1] = addedIndices[face_index + 2] - 1;
		addedIndices[face_index + 5] = addedIndices[face_index + 2] + 1;
	}

	indices.insert(indices.end(), addedIndices.begin(), addedIndices.end());
}

void MeshRenderManager::update(Engine::Component::Camera& activeCamera) {
	//delete removed meshes data
	if (MeshUpdater::deletedMeshes.size()) {
		for (auto& deleted_mesh_info : MeshUpdater::deletedMeshes) {
			//GET MESH
			Mesh& deleted_mesh = meshMgr->get_mesh(deleted_mesh_info.second);
			//GET SUBMESHES
			for (auto& submesh_info : deleted_mesh.subMeshes) {
				//remove model matrix
				//meshesInstanceData.remove(submesh_id);
				//decrease amount of instances of this submesh
				drawCommands[submesh_info.first].instanceCount--;
			}
			std::cout << "\t---Mesh model matrix (id = " << deleted_mesh_info.first << ") removed---" << std::endl;
		}
		MeshUpdater::deletedMeshes.clear();
		update_indirect_base_instances();
	}
	//add new meshes data
	if (MeshUpdater::addedMeshes.size() > 0) {
		for (auto& obj_id : MeshUpdater::addedMeshes) {
			Engine::Component::Transform* tr_ptr = trSystem->get_component(obj_id);
			Engine::Component::MeshComponent* mc_ptr = mcSystem->get_component(obj_id);
			Mesh& addedMesh = meshMgr->get_mesh(mc_ptr->mesh_id);
			for (auto& submesh_info : addedMesh.subMeshes) {
				//Material& material = matMgr->get_material(subMeshMgr->get(submesh_id).material_id);
				//meshesInstanceData.add(SubMeshInstanceData(tr_ptr->worldMatrix, material.id), submesh_id);
				drawCommands[submesh_info.first].instanceCount++;
				//std::cout << "--Submesh added to renderer [id = " << submesh_id << "] Material id = " << material.id << ", albedo id = " << material.albedoID << std::endl;
			}
		}
		MeshUpdater::addedMeshes.clear();
		update_indirect_base_instances();
	}

	//update existing meshes data
	uint32_t size = mcSystem->meshComponents.size();
	submesh_inst_key = !submesh_inst_key; //change key to update instance offsets correctly

	//get local pointers to boost speed
	auto* meshComps = mcSystem->meshComponents.data();
	auto* drawCmds = drawCommands.data();
	auto* offsets = submeshOffsets.data();
	auto* instData = submeshesData.data();

	for (uint32_t i = 0; i < size; i++) {

		Engine::Component::MeshComponent& mc = meshComps[i];
		if (!mc.transform_ptr) continue;
		const Mesh& mesh = meshMgr->get_mesh(mc.mesh_id);
		const glm::mat4& worldMatrix = mc.transform_ptr->worldMatrix;

		for (auto& submesh_info : mesh.subMeshes) {
			auto& offsetData = offsets[submesh_info.first];
			if (submesh_inst_key != offsetData.second) {
				offsetData.second = submesh_inst_key;
				offsetData.first = 0;
			}

			//get subMeshData based on baseInstance + instance_offset
			SubMeshInstanceData& subMeshData = instData[drawCmds[submesh_info.first].baseInstance + offsetData.first++];
			subMeshData.modelMatrix = worldMatrix;
			subMeshData.materialID = submesh_info.second;
		}
	}

	//update indirect buffer
	indirectBuffer->updateData(drawCommands.data(), sizeof(DrawElementsIndirectCommand) * drawCommands.size());
	//update storage buffer
	storageBuffer->updateData(submeshesData.data(), size * sizeof(SubMeshInstanceData));

	//update camera data
	basicUniformData.view = activeCamera.get_view_matrix();
	basicUniformData.projection = activeCamera.get_projection_matrix();
	uniformBuffer->updateData(&basicUniformData, sizeof(BasicShaderUBData));
}

void MeshRenderManager::render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	renderer->renderMultiIndirectData(shader, vertexArray, vertexBuffer, indexBuffer, indirectBuffer, drawCommands.size(), sizeof(DrawElementsIndirectCommand));
}

void MeshRenderManager::load_submesh(
	std::vector<uint32_t>& submesh_indices,
	std::vector<SubMeshVertex>& submesh_vertices,
	SubMesh& submesh
) {
	drawCommands.emplace_back(
		DrawElementsIndirectCommand(submesh.indexCount, 0, index_current_global_size, vertex_current_global_size, 0)
	);
	//add new data at the end to buffers
	vertexBuffer->update_data(submesh_vertices.data(), sizeof(SubMeshVertex) * submesh_vertices.size(), sizeof(SubMeshVertex) * vertex_current_global_size);
	indexBuffer->update_data(submesh_indices.data(), sizeof(uint32_t) * submesh_indices.size(), sizeof(uint32_t) * index_current_global_size);
	//simply update indirectBuffer
	indirectBuffer->updateData(drawCommands.data(), sizeof(DrawElementsIndirectCommand) * drawCommands.size(), 0);

	index_current_global_size += submesh.indexCount;
	vertex_current_global_size += submesh.vertexCount;
}

void MeshRenderManager::load_submeshes(
	std::vector<uint32_t>& submeshes_indices,
	std::vector<SubMeshVertex>& submeshes_vertices,
	std::vector<SubMesh>& submeshes
) {
	//add new data at the end to buffers
	vertexBuffer->update_data(submeshes_vertices.data(), sizeof(SubMeshVertex) * submeshes_vertices.size(), sizeof(SubMeshVertex) * vertex_current_global_size);
	indexBuffer->update_data(submeshes_indices.data(), sizeof(uint32_t) * submeshes_indices.size(), sizeof(uint32_t) * index_current_global_size);

	for (SubMesh& submesh : submeshes) {
		drawCommands.emplace_back(
			DrawElementsIndirectCommand(submesh.indexCount, 0, index_current_global_size, vertex_current_global_size, 0)
		);
		index_current_global_size += submesh.indexCount;
		vertex_current_global_size += submesh.vertexCount;
	}

	//simply update indirectBuffer
	indirectBuffer->updateData(drawCommands.data(), sizeof(DrawElementsIndirectCommand) * drawCommands.size(), 0);
}

void MeshRenderManager::update_indirect_base_instances() {
	uint32_t offset = 0;
	for (auto& command : drawCommands) {
		command.baseInstance = offset;
		offset += command.instanceCount;
	}
}
