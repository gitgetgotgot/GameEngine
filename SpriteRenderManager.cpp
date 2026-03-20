#include "SpriteRenderManager.h"

void SpriteRenderManager::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	ShaderDesc desc("", "Shaders/OpenGL/Sprite Vert.glsl", "Shaders/OpenGL/Sprite Frag.glsl");
	shader = renderer->createShader(desc);
	shader->bind();

	vertexArray = renderer->createVertexArray();
	vertexArray->bind();

	vertexBuffer = renderer->createVertexBuffer();
	vertexBuffer->set_data(spriteInstanceVertices, sizeof(spriteInstanceVertices), renderer->STATIC_USAGE);

	vertexArray->add_attribute(VertexAttribute(0, 3, 0, sizeof(glm::vec3)), vertexBuffer);

	indexBuffer = renderer->createIndexBuffer();
	indexBuffer->set_data(spriteInstanceIndices, sizeof(spriteInstanceIndices), renderer->STATIC_USAGE);

	vertexArray->unbind();
	indexBuffer->unbind();

	uniformBuffer = renderer->createUniformBuffer();
	uniformBuffer->bind(0);
	uniformBuffer->setData(&basicUniformData, sizeof(BasicShaderUBData));

	storageBuffer = renderer->createStorageBuffer();
	storageBuffer->bind(0);
	storageBuffer->setData(nullptr, sizeof(SpriteInstanceData) * 100);

	trSystem = static_cast<TransformSystem*>(ComponentSystemsCore::get_Instance()->get_system<Engine::Component::Transform>());
	srSystem = static_cast<SpriteRendererSystem*>(ComponentSystemsCore::get_Instance()->get_system<Engine::Component::SpriteRenderer>());
}

void SpriteRenderManager::update(Engine::Component::Camera& activeCamera) {
	//delete removed sprites data
	if (SpriteUpdater::deletedSprites.size()) {
		for (auto& obj_id : SpriteUpdater::deletedSprites) {
			spritesData.remove(obj_id);
			std::cout << "\t---Sprite data (id = " << obj_id << ") removed---" << std::endl;
		}
		SpriteUpdater::deletedSprites.clear();
	}
	//add new sprites data
	if (SpriteUpdater::addedSprites.size() > 0) {
		for (auto& obj_id : SpriteUpdater::addedSprites) {
			Engine::Component::Transform* tr_ptr = trSystem->get_component(obj_id);
			Engine::Component::SpriteRenderer* sr_ptr = srSystem->get_component(obj_id);
			spritesData.add(SpriteInstanceData(sr_ptr->sprite, tr_ptr->worldMatrix), obj_id);
		}
		SpriteUpdater::addedSprites.clear();
	}
	//update existing sprites data
	int size = srSystem->spriteRenderers.size();
	for (int i = 0; i < size; i++) {
		Engine::Component::SpriteRenderer& sr = srSystem->spriteRenderers[i];
		spritesData.get(sr.obj_id)->modelMatrix = trSystem->transforms.get(sr.obj_id)->worldMatrix;
	}
	//update storage buffer
	storageBuffer->updateData(spritesData.data(), spritesData.size() * sizeof(SpriteInstanceData));

	//update camera data
	basicUniformData.view = activeCamera.get_view_matrix();
	basicUniformData.projection = activeCamera.get_projection_matrix();
	uniformBuffer->updateData(&basicUniformData, sizeof(BasicShaderUBData));
}

void SpriteRenderManager::render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	renderer->renderInstancedData(shader, vertexArray, vertexBuffer, indexBuffer, 6, spritesData.size());
}