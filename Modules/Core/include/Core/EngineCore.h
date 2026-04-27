#pragma once
#include <memory>

#ifdef _WIN32
//#include "DirectX_Renderer.h"
#endif
#include <Rendering/OpenGL/OpenGL_Renderer.h>
//#include "Vulkan_Renderer.h"

#include <SystemModule/InputHandler.h>
#include <SystemModule/TimeManager.h>
#include <Utility/TextBufferBuilder.h>

#include <CustomScripts/PlayerController.h>
#include <RenderManagers/SpriteRenderManager.h>
#include <RenderManagers/UI_RenderManager.h>
#include <Project/ModelManager.h>
#include <Objects/ComponentSystems/CameraManager.h>

#include <Editor/Editor.h>

namespace EC = Engine::Component;
namespace ES = Engine::Systems;

class EngineCore {
public:
	void main_init();
	void processMainLoop();
	void shutdown();
private:
	void update();
	void render();
	void input_endFrame();

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
	Engine::Object::ObjectManager* objectManager = nullptr;
	Engine::Object::ScriptManager* scriptManager = nullptr;
	CameraManager* cameraManager = nullptr;
	ES::ComponentSystemsCore* compSysCore = nullptr;
	ES::UI_ComponentSystemsCore* uiCompSysCore = nullptr;
	SpriteRenderManager* spriteRenderer = nullptr;
	MeshRenderManager* meshRenderer = nullptr;
	UI_RenderManager* uiRenderer = nullptr;

	TextureManager* texManager = nullptr;
	FontManager* fontManager = nullptr;
	MaterialManager* matManager = nullptr;
	SpriteManager* spriteManager = nullptr;
	MeshManager* meshManager = nullptr;
	Engine::Models::ModelManager* modelManager = nullptr;

	Engine::Editor::Editor* editor = nullptr;

	//test
	EC::Component_Ptr<EC::Camera> cameraMain;
	EC::Component_Ptr<EC::Transform> camera_transform;
	bool can_control_camera = false;

	void test_method();
};