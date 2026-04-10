#pragma once
#include <memory>

#ifdef _WIN32
//#include "DirectX_Renderer.h"
#endif
#include "OpenGL_Renderer.h"
//#include "Vulkan_Renderer.h"

#include "InputHandler.h"
#include "TimeManager.h"
#include "SimpleTextRenderer.h"
#include "TextBufferBuilder.h"

#include "ObjectManager.h"
#include "PlayerController.h"
#include "SpriteRenderManager.h"
#include "ModelManager.h"
#include "UI_RenderManager.h"
#include "CameraManager.h"

#include "UI_TransformSystem.h"
#include "ImageSystem.h"
#include "ButtonSystem.h"
#include "SDF_TextSystem.h"

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

	std::unique_ptr<SimpleTextRenderer> simpleTextRenderer; //temporary

	//test
	EC::Component_Ptr<EC::Camera> cameraMain;
	EC::Component_Ptr<EC::Transform> camera_transform;
	bool can_control_camera = false;

	void test_method();
};