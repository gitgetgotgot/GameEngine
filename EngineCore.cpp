#include "EngineCore.h"

#include "ObjectManager.h"
#include "TransformSystem.h"
#include "CameraSystem.h"

void EngineCore::main_init() {
	init_Graphic_API();

	init_Input_Handler();
	
	init_component_systems();

	init_UI_component_systems();

	//textManager
	textManager = std::make_unique<TextManager>(renderer);

	//time manager
	timeManager = std::make_unique<TimeManager>();

	//default camera
	cameraManager = CameraManager::get_Instance();
	cameraManager->init();

	//sprite renderer
	spriteRenderer = SpriteRenderManager::get_Instance();
	spriteRenderer->init(renderer);

	//texture manager
	texManager = TextureManager::get_Instance();
	texManager->load("Resources/Textures/crate.jpg", false, renderer);
	texManager->load("Resources/Textures/stone.png", false, renderer);

	//mesh manager
	meshManager = MeshManager::get_Instance();
	meshManager->init_basic_meshes();

	//sprite manager

	spriteManager = SpriteManager::get_Instance();
	spriteManager->add(0);
	spriteManager->add(1);

	//mesh renderer
	meshRenderer = MeshRenderManager::get_Instance();
	meshRenderer->init(renderer);

	//UI renderer
	uiRenderer = UI_RenderManager::get_Instance();
	uiRenderer->init(renderer);
	
	//object manager
	objectManager = Engine::Object::ObjectManager::get_Instance();

	///TEST
	Engine::Object::object_ptr obj = objectManager->InstantiateObject();
	obj->add_script<PlayerController>();

	Engine::Object::object_ptr cameraObj;
	cameraObj = objectManager->InstantiateObject();

	auto c = cameraObj->add_component<Engine::Component::Camera>();
	camera_transform = cameraObj->add_component<Engine::Component::Transform>();
	c->set_perspective(0.1f, 100.f);
	cameraManager->set_active_camera(cameraObj);

	Engine::Object::object_ptr camera_holder_obj = objectManager->InstantiateObject();
	camera_transform = camera_holder_obj->add_component<Engine::Component::Transform>();
	camera_holder_obj->add_child_object(*cameraObj);

	Engine::Object::object_ptr obj_child = objectManager->InstantiateObject();
	Engine::Component::component_ptr<Engine::Component::Transform> child_tr = obj_child->add_component<Engine::Component::Transform>();
	child_tr->translateLocal(1.5f, 1.5f, 0.f);
	Engine::Component::component_ptr<Engine::Component::MeshRenderer> child_mr = obj_child->add_component<Engine::Component::MeshRenderer>();
	child_mr->mesh = MeshManager::get_Instance()->get_mesh(1);

	obj->add_child_object(*obj_child);

	//canvas test
	auto canvas_obj = objectManager->InstantiateObject();
	auto canvas = canvas_obj->add_ui_component<Engine::UI::Canvas>();
	canvas_obj->add_ui_component<Engine::UI::UI_Transform>();

	auto image_obj = objectManager->InstantiateObject();
	image_obj->add_ui_component<Engine::UI::UI_Transform>();
	auto image = image_obj->add_ui_component<Engine::UI::Image>();
	image->sprite = SpriteManager::get_Instance()->get(0);

	canvas_obj->add_child_UI_object(*image_obj);
	canvas->attach_ui_component<Engine::UI::Image>(image_obj.get_id());
}

void EngineCore::processMainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		update();

		render();

	}
}

void EngineCore::shutdown() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void EngineCore::update() {

	timeManager->update();

	compSysCore->update();

	cameraManager->update_active_camera();

	spriteRenderer->update(cameraManager->get_active_camera());
	meshRenderer->update(cameraManager->get_active_camera());
	uiRenderer->update();

	objectManager->Update();

	if (SystemContext::keyBoard.keyStates[GLFW_KEY_C]) {
		SystemContext::keyBoard.keyStates[GLFW_KEY_C] = false;
		can_control_camera = !can_control_camera;
	}

	if (can_control_camera) {
		if (SystemContext::keyBoard.keyStates[GLFW_KEY_W]) {
			camera_transform->translateDirectionally(0.f, 0.f, TimeManager::deltaTime);
		}
		if (SystemContext::keyBoard.keyStates[GLFW_KEY_A]) {
			camera_transform->translateDirectionally(-TimeManager::deltaTime, 0.f, 0.f);
		}
		if (SystemContext::keyBoard.keyStates[GLFW_KEY_S]) {
			camera_transform->translateDirectionally(0.f, 0.f, -TimeManager::deltaTime);
		}
		if (SystemContext::keyBoard.keyStates[GLFW_KEY_D]) {
			camera_transform->translateDirectionally(TimeManager::deltaTime, 0.f, 0.f);
		}
		if (SystemContext::keyBoard.keyStates[GLFW_KEY_LEFT_SHIFT]) {
			camera_transform->translateLocal(0.f, TimeManager::deltaTime, 0.f);
		}
		if (SystemContext::keyBoard.keyStates[GLFW_KEY_LEFT_CONTROL]) {
			camera_transform->translateLocal(0.f, -TimeManager::deltaTime, 0.f);
		}

		if (SystemContext::keyBoard.keyStates[GLFW_KEY_LEFT]) {
			camera_transform->rotate(false, true, false, -TimeManager::deltaTime * 30.0f);
		}
		else if (SystemContext::keyBoard.keyStates[GLFW_KEY_RIGHT]) {
			camera_transform->rotate(false, true, false, TimeManager::deltaTime * 30.0f);
		}
		else if (SystemContext::keyBoard.keyStates[GLFW_KEY_UP]) {
			camera_transform->rotate(true, false, false, -TimeManager::deltaTime * 30.0f);
		}
		else if (SystemContext::keyBoard.keyStates[GLFW_KEY_DOWN]) {
			camera_transform->rotate(true, false, false, TimeManager::deltaTime * 30.0f);
		}

	}
}

void EngineCore::render() {
	renderer->clear(0.f, 0.f, 0.f);
	
	texManager->get(1)->bind();

	spriteRenderer->render(renderer);

	meshRenderer->render(renderer);

	uiRenderer->render(renderer);

	//// temporary text test
	textBuilder.add_int(timeManager->get_current_FPS());
	textManager->add_text_to_buffer(textBuilder.data(), 0.2f, glm::vec3(-SystemContext::screen.ratio, 0.8f, 0.f), timeManager->getRainbowColor(), "Verdana");
	textBuilder.reset();

	if (SystemContext::mouse.left_button) {
		textManager->add_text_to_buffer("left pressed", 0.2f, glm::vec3(-1.5f, -0.8f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	else if (!SystemContext::mouse.left_button) {
		textManager->add_text_to_buffer("left not pressed", 0.2f, glm::vec3(-1.5f, -0.8f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	if (SystemContext::mouse.right_button) {
		textManager->add_text_to_buffer("right pressed", 0.2f, glm::vec3(-1.5f, -0.6f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	else if (!SystemContext::mouse.right_button) {
		textManager->add_text_to_buffer("right not pressed", 0.2f, glm::vec3(-1.5f, -0.6f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	textManager->render_buffered_text(renderer);
	////

	renderer->present();
}

void EngineCore::init_component_systems() {
	compSysCore = ComponentSystemsCore::get_Instance();
	compSysCore->register_system<Engine::Component::Transform, TransformSystem>();
	compSysCore->register_system<Engine::Component::SpriteRenderer, SpriteRendererSystem>();
	compSysCore->register_system<Engine::Component::MeshRenderer, MeshRendererSystem>();
	compSysCore->register_system<Engine::Component::Camera, CameraSystem>();
}

void EngineCore::init_UI_component_systems() {
	uiCompSysCore = UI_ComponentSystemsCore::get_Instance();
	uiCompSysCore->register_system<Engine::UI::Canvas, CanvasSystem>();
	uiCompSysCore->register_system<Engine::UI::UI_Transform, UI_TransformSystem>();
	uiCompSysCore->register_system<Engine::UI::Image, ImageSystem>();
}

void EngineCore::init_Graphic_API() {
	glfwInit();

	if (render_api == RENDER_API::OpenGL_API) {
		//version of OpenGL (version 4.6)
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		//using the core profile for only modern functions
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//window
		window = glfwCreateWindow(2560, 1440, "Engine Window", NULL, NULL);
		renderer = std::make_unique<Engine::Graphics::OpenGL_Renderer>(window);
		//context
		glfwMakeContextCurrent(window);
		glfwSetWindowAspectRatio(window, 16, 9);
		//glad
		gladLoadGL();
		//glfwSwapInterval(0);
	}
	else if (render_api == RENDER_API::DirectX_API) {
		//renderer = std::make_unique<DirectX_Renderer>();
	}
	else if (render_api == RENDER_API::Vulkan_API) {
		//renderer = std::make_unique<Vulkan_Renderer>();
	}
	//init renderer
	renderer->init();
}

void EngineCore::init_Input_Handler() {
	InputHandler::setGLFWwindowCallbacks(window);
}
