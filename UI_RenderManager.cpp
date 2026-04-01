#include "UI_RenderManager.h"
#include "SystemContext.h"
#include <glm/gtc/matrix_transform.hpp>

void UI_RenderManager::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	ShaderDesc desc("", "Shaders/OpenGL/UI Vert.glsl", "Shaders/OpenGL/UI Frag.glsl");
	shader = renderer->createShader(desc);
	shader->bind();

	vertexArray = renderer->createVertexArray();
	vertexArray->bind();

	vertexBuffer = renderer->createVertexBuffer();
	vertexBuffer->set_data(UI_InstanceVertices, sizeof(UI_InstanceVertices), renderer->STATIC_USAGE);

	vertexArray->add_attribute(VertexAttribute(0, 3, 0, sizeof(glm::vec3)), vertexBuffer);

	indexBuffer = renderer->createIndexBuffer();
	indexBuffer->set_data(UI_InstanceIndices, sizeof(UI_InstanceIndices), renderer->STATIC_USAGE);

	vertexArray->unbind();
	indexBuffer->unbind();

	uniformBuffer = renderer->createUniformBuffer();
	uniformBuffer->bind(2);
	basicUniformData.view = glm::mat4(1.0f);
	basicUniformData.projection = glm::orthoLH(-SystemContext::screen.ratio, SystemContext::screen.ratio, -1.0f, 1.0f, 0.f, 10.f); //SHOULD MAKE THIS CHANGEABLE
	uniformBuffer->setData(&basicUniformData, sizeof(BasicShaderUBData));

	storageBuffer = renderer->createStorageBuffer();
	storageBuffer->bind(2);
	storageBuffer->setData(nullptr, sizeof(Engine::Systems::UI_InstanceData) * 100); //allocate memory for 100 sprites

	uiData.reserve(100); //allocate memory in vector for 100 sprites

	canvasSystem = static_cast<Engine::Systems::CanvasSystem*>(Engine::Systems::UI_ComponentSystemsCore::get_Instance()->get_system<Engine::UI::Canvas>());
	ui_transformSystem = static_cast<Engine::Systems::UI_TransformSystem*>(Engine::Systems::UI_ComponentSystemsCore::get_Instance()->get_system<Engine::UI::UI_Transform>());
}

void UI_RenderManager::update() {
	uiData.resize(0);
	//update UI data for each enabled canvas
	auto& canvases = canvasSystem->canvases;
	for (auto& it : canvases) {
		Engine::UI::Canvas& canvas = it.second;
		if (canvas.isEnabled) {
			canvas.update_ui_data_buffer(uiData);
		}
	}

	//update storage buffer
	storageBuffer->updateData(uiData.data(), uiData.size() * sizeof(Engine::Systems::UI_InstanceData));

	if (uiData.size() > 2) {
		std::cout << "UI data size > 2" << std::endl;
	}
}

void UI_RenderManager::render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	renderer->renderInstancedData(shader, vertexArray, vertexBuffer, indexBuffer, 6, uiData.size());
}