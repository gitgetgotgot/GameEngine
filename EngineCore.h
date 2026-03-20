#pragma once
#include <memory>

#ifdef _WIN32
//#include "DirectX_Renderer.h"
#endif
#include "OpenGL_Renderer.h"
//#include "Vulkan_Renderer.h"

#include "InputHandler.h"
#include "TimeManager.h"
#include "TextManager.h"
#include "TextBufferBuilder.h"

#include "ObjectManager.h"
#include "PlayerController.h"
#include "SpriteRenderManager.h"
#include "MeshRenderManager.h"
#include "UI_RenderManager.h"
#include "CameraManager.h"
#include "TextureManager.h"

#include "UI_TransformSystem.h"
#include "ImageSystem.h"

struct BasicVertex {
	glm::vec3 pos;
	glm::vec2 UV;
};

class EngineCore {
public:
	void main_init();
	void processMainLoop();
	void shutdown();
private:
	void update();
	void render();

	//initializers
	void init_Graphic_API();
	void init_Input_Handler();
	void init_component_systems();
	void init_UI_component_systems();
	
	GLFWwindow* window;
	TextBufferBuilder textBuilder;
	std::unique_ptr<TimeManager> timeManager;

	RENDER_API render_api = RENDER_API::OpenGL_API;

	std::unique_ptr<Engine::Graphics::RendererInterface> renderer;
	Engine::Object::ObjectManager* objectManager;
	CameraManager* cameraManager;
	ComponentSystemsCore* compSysCore;
	UI_ComponentSystemsCore* uiCompSysCore;
	SpriteRenderManager* spriteRenderer;
	MeshRenderManager* meshRenderer;
	UI_RenderManager* uiRenderer;

	TextureManager* texManager;
	SpriteManager* spriteManager;
	MeshManager* meshManager;

	std::unique_ptr<TextManager> textManager; //temporary

	//test
	Engine::Component::component_ptr<Engine::Component::Camera> cameraMain;
	Engine::Component::component_ptr<Engine::Component::Transform> camera_transform;
	bool can_control_camera = false;
};