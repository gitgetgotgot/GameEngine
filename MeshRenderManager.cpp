#include "MeshRenderManager.h"

void MeshRenderManager::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	std::vector<uint32_t> indices;
	std::vector<MeshVertex> vertices;
	load_basic_meshes(indices, vertices);

	ShaderDesc desc("", "Shaders/OpenGL/Mesh Vert.glsl", "Shaders/OpenGL/Mesh Frag.glsl");
	shader = renderer->createShader(desc);
	shader->bind();

	vertexArray = renderer->createVertexArray();
	vertexArray->bind();

	vertexBuffer = renderer->createVertexBuffer();
	vertexBuffer->set_data(vertices.data(), sizeof(MeshVertex) * vertices.size(), renderer->STATIC_USAGE);

	vertexArray->add_attribute(VertexAttribute(0, 3, offsetof(MeshVertex, MeshVertex::pos), sizeof(MeshVertex)), vertexBuffer);
	vertexArray->add_attribute(VertexAttribute(1, 3, offsetof(MeshVertex, MeshVertex::normal), sizeof(MeshVertex)), vertexBuffer);
	vertexArray->add_attribute(VertexAttribute(2, 2, offsetof(MeshVertex, MeshVertex::UV), sizeof(MeshVertex)), vertexBuffer);
	vertexArray->add_attribute(VertexAttribute(3, 1, offsetof(MeshVertex, MeshVertex::material_id), sizeof(MeshVertex)), vertexBuffer);

	indexBuffer = renderer->createIndexBuffer();
	indexBuffer->set_data(indices.data(), sizeof(uint32_t) * indices.size(), renderer->STATIC_USAGE);

	vertexArray->unbind();
	indexBuffer->unbind();

	uniformBuffer = renderer->createUniformBuffer();
	uniformBuffer->bind(1);
	uniformBuffer->setData(&basicUniformData, sizeof(BasicShaderUBData));

	storageBuffer = renderer->createStorageBuffer();
	storageBuffer->bind(1);
	storageBuffer->setData(nullptr, sizeof(glm::mat4) * 100);

	indirectBuffer = renderer->createIndirectBuffer();
	indirectBuffer->setData(drawCommands.data(), sizeof(DrawElementsIndirectCommand) * drawCommands.size());

	trSystem = static_cast<TransformSystem*>(ComponentSystemsCore::get_Instance()->get_system<Engine::Component::Transform>());
	mrSystem = static_cast<MeshRendererSystem*>(ComponentSystemsCore::get_Instance()->get_system<Engine::Component::MeshRenderer>());
}

void MeshRenderManager::load_basic_meshes(std::vector<uint32_t>& indices, std::vector<MeshVertex>& vertices) {
	//add draw command and update main buffers
	uint32_t index_offset = 0, vertex_offset = 0;
	Mesh* mesh;
	for (int i = 0; i < 4; i++) {
		mesh = MeshManager::get_Instance()->get_mesh(i);

		vertices.insert(vertices.end(), mesh->vertices.begin(), mesh->vertices.end());
		indices.insert(indices.end(), mesh->indices.begin(), mesh->indices.end());

		drawCommands.emplace_back(DrawElementsIndirectCommand(mesh->indices.size(), 0, index_offset, vertex_offset, 0)); //SHOULD THINK ABOUT FIRST INSTANCE
		index_offset += mesh->indices.size();
		vertex_offset += mesh->vertices.size();
	}
	drawCommands[2].baseInstance = 1;
	//after loading buffers and commands, shrink_to_fit to free unused memory in temporary buffers
	vertices.shrink_to_fit();
	indices.shrink_to_fit();
	drawCommands.shrink_to_fit();
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
			//delete matrix with object id
			meshModelMatrices.remove(deleted_mesh_info.first);
			//decrease amount of instances of this mesh with mesh id
			drawCommands[deleted_mesh_info.second].instanceCount--;
			
			std::cout << "\t---Mesh model matrix (id = " << deleted_mesh_info.first << ") removed---" << std::endl;
		}
		MeshUpdater::deletedMeshes.clear();
	}
	//add new meshes data
	if (MeshUpdater::addedMeshes.size() > 0) {
		for (auto& obj_id : MeshUpdater::addedMeshes) {
			Engine::Component::Transform* tr_ptr = trSystem->get_component(obj_id);
			Engine::Component::MeshRenderer* mr_ptr = mrSystem->get_component(obj_id);
			meshModelMatrices.add(std::move(tr_ptr->worldMatrix), obj_id);
			
			drawCommands[mr_ptr->mesh->id].instanceCount++;
		}
		MeshUpdater::addedMeshes.clear();
	}
	//update existing meshes data
	uint32_t size = mrSystem->meshRenderers.size();
	for (int i = 0; i < size; i++) {
		Engine::Component::MeshRenderer& mr = mrSystem->meshRenderers[i];
		*meshModelMatrices.get(mr.obj_id) = trSystem->transforms.get(mr.obj_id)->worldMatrix;
	}

	//update indirect buffer
	indirectBuffer->updateData(drawCommands.data(), sizeof(DrawElementsIndirectCommand) * drawCommands.size());
	//update storage buffer
	storageBuffer->updateData(meshModelMatrices.data(), meshModelMatrices.size() * sizeof(glm::mat4));

	//update camera data
	basicUniformData.view = activeCamera.get_view_matrix();
	basicUniformData.projection = activeCamera.get_projection_matrix();
	uniformBuffer->updateData(&basicUniformData, sizeof(BasicShaderUBData));
}

void MeshRenderManager::render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	renderer->renderMultiIndirectData(shader, vertexArray, vertexBuffer, indexBuffer, indirectBuffer, drawCommands.size(), sizeof(DrawElementsIndirectCommand));
}