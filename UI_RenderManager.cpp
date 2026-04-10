#include "UI_RenderManager.h"
#include "SystemContext.h"
#include <glm/gtc/matrix_transform.hpp>

void UI_RenderManager::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	ShaderDesc desc("", "Shaders/OpenGL/UI Vert.glsl", "Shaders/OpenGL/UI Frag.glsl");
	ShaderDesc desc2("", "Shaders/OpenGL/SDF_Text Vert.glsl", "Shaders/OpenGL/SDF_Text Frag.glsl");
	shader = renderer->createShader(desc);
	text_shader = renderer->createShader(desc2);

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
	basicUniformData.projection = glm::orthoLH(-SystemContext::screen.ratio, SystemContext::screen.ratio, -1.0f, 1.0f, -10.f, 10.f); //SHOULD MAKE THIS CHANGEABLE
	uniformBuffer->setData(&basicUniformData, sizeof(BasicShaderUBData));

	storageBuffer = renderer->createStorageBuffer();
	storageBuffer->bind(2);
	storageBuffer->setData(nullptr, sizeof(Engine::Systems::UI_InstanceData) * MAX_UI_ELEMENTS);

	storageTextBuffer = renderer->createStorageBuffer();
	storageTextBuffer->bind(5);
	storageTextBuffer->setData(nullptr, sizeof(Engine::Systems::UI_InstanceData) * MAX_TEXT_SYMBOLS);

	uiData.reserve(MAX_UI_ELEMENTS);
	uiTextData.reserve(MAX_TEXT_SYMBOLS);

	canvasSystem = static_cast<Engine::Systems::CanvasSystem*>(Engine::Systems::UI_ComponentSystemsCore::get_Instance()->get_system<Engine::UI::Canvas>());
	ui_transformSystem = static_cast<Engine::Systems::UI_TransformSystem*>(Engine::Systems::UI_ComponentSystemsCore::get_Instance()->get_system<Engine::UI::UI_Transform>());
}

void UI_RenderManager::update() {
	uiData.resize(0);
	uiTextData.resize(0);
	//update UI data for each enabled canvas
	auto& canvases = canvasSystem->canvases;
	for (auto& it : canvases) {
		Engine::UI::Canvas& canvas = it.second;
		if (canvas.isEnabled) {
			canvas.update_ui_data_buffer(uiData, uiTextData);
		}
	}
	//update storage buffer
	storageBuffer->updateData(uiData.data(), uiData.size() * sizeof(Engine::Systems::UI_InstanceData));
	//update text storage buffer
	storageTextBuffer->updateData(uiTextData.data(), uiTextData.size() * sizeof(Engine::Systems::UI_InstanceData));
}

void UI_RenderManager::render(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	renderer->renderInstancedData(shader, vertexArray, vertexBuffer, indexBuffer, 6, uiData.size());
	renderer->renderInstancedData(text_shader, vertexArray, vertexBuffer, indexBuffer, 6, uiTextData.size());
}