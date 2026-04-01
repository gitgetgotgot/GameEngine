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

	//texture manager
	texManager = TextureManager::get_Instance();
	texManager->init(renderer);

	texManager->create_texture_from_file("Resources/Textures/default.png", false); //default texture
	MaterialManager::get_Instance()->add_material(); //default material

	texManager->create_texture_from_file("Resources/Textures/crate.jpg", false);
	texManager->create_texture_from_file("Resources/Textures/stone.png", false);

	//mesh manager
	meshManager = MeshManager::get_Instance();

	//sprite manager
	spriteManager = SpriteManager::get_Instance();
	spriteManager->add(0);
	spriteManager->add(1);

	//sprite renderer
	spriteRenderer = SpriteRenderManager::get_Instance();
	spriteRenderer->init(renderer);

	//mesh renderer
	meshRenderer = MeshRenderManager::get_Instance();
	meshRenderer->init(renderer);

	//UI renderer
	uiRenderer = UI_RenderManager::get_Instance();
	uiRenderer->init(renderer);
	
	//object manager
	objectManager = Engine::Object::ObjectManager::get_Instance();

	//model manager
	modelManager = Engine::Models::ModelManager::get_Instance();
	modelManager->init();
	modelManager->load_plane_mesh();
	modelManager->load_cube_mesh();
	modelManager->load_pyramid_mesh();
	modelManager->load_cylinder_mesh();
	
	test_method();
}

void EngineCore::test_method() {
	std::string paths[] = {
		"Resources/Models/1994_toyota_supra_mk_iv_bomex_fast__furious.glb",
		"Resources/Models/1965_ac_shelby_cobra_427__www.vecarz.com.glb",
		"Resources/Models/shelby_cobra_427.glb",
		"Resources/Models/genshin_impact_-_furina.glb",
		"Resources/Models/simple_axe.glb"
	};
	modelManager->load_model(paths[3]);

	///TEST
	Engine::Object::object_ptr obj = objectManager->InstantiateObject();
	auto playerController = obj->add_script<PlayerController>();

	Engine::Object::object_ptr cameraObj;
	cameraObj = objectManager->InstantiateObject();

	auto c = cameraObj->add_component<Engine::Component::Camera>();
	camera_transform = cameraObj->add_component<Engine::Component::Transform>();
	c->set_perspective(0.1f, 100.f);
	cameraManager->set_active_camera(cameraObj);

	Engine::Object::object_ptr camera_holder_obj = objectManager->InstantiateObject();
	camera_transform = camera_holder_obj->add_component<Engine::Component::Transform>();
	camera_holder_obj->add_child_object(*cameraObj);

	/*Engine::Object::object_ptr obj_child = objectManager->InstantiateObject();
	Engine::Component::component_ptr<Engine::Component::Transform> child_tr = obj_child->add_component<Engine::Component::Transform>();
	child_tr->translateLocal(1.5f, 1.5f, 0.f);
	Engine::Component::component_ptr<Engine::Component::MeshComponent> child_mr = obj_child->add_component<Engine::Component::MeshComponent>();
	child_mr->mesh = MeshManager::get_Instance()->get_mesh(1);

	obj->add_child_object(*obj_child);*/

	//canvas test
	auto canvas_obj = objectManager->InstantiateObject();
	auto canvas = canvas_obj->add_ui_component<Engine::UI::Canvas>();
	canvas_obj->add_ui_component<Engine::UI::UI_Transform>();

	auto image_obj = objectManager->InstantiateObject();
	auto image_tr = image_obj->add_ui_component<Engine::UI::UI_Transform>();
	auto image = image_obj->add_ui_component<Engine::UI::Image>();
	image->set_sprite(&SpriteManager::get_Instance()->get(0));
	image_tr->translateLocal(1.0f, 0.f, 0.f);
	//image_tr->rotate(0, 0, 1, 30.0f);
	//image->flipX(true);

	playerController->set_test_image(image);

	canvas_obj->add_child_UI_object(*image_obj);
	canvas->attach_ui_component<Engine::UI::Image>(image_obj.get_id());

	modelManager->create_model_object(0);
}

void EngineCore::processMainLoop() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		update();

		render();

		input_endFrame();
	}
}

void EngineCore::shutdown() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void EngineCore::update() {

	timeManager->update();

	compSysCore->update();

	uiCompSysCore->update();

	cameraManager->update_active_camera();

	spriteRenderer->update(cameraManager->get_active_camera());
	meshRenderer->update(cameraManager->get_active_camera());
	uiRenderer->update();

	objectManager->Update();

	if (SystemContext::keyBoard.key_is_pressed(GLFW_KEY_C)) {
		can_control_camera = !can_control_camera;
	}

	if (can_control_camera) {
		if (SystemContext::keyBoard.key_is_held(KeyW)) {
			camera_transform->translateDirectionally(0.f, 0.f, TimeManager::deltaTime);
		}
		if (SystemContext::keyBoard.key_is_held(KeyA)) {
			camera_transform->translateDirectionally(-TimeManager::deltaTime, 0.f, 0.f);
		}
		if (SystemContext::keyBoard.key_is_held(KeyS)) {
			camera_transform->translateDirectionally(0.f, 0.f, -TimeManager::deltaTime);
		}
		if (SystemContext::keyBoard.key_is_held(KeyD)) {
			camera_transform->translateDirectionally(TimeManager::deltaTime, 0.f, 0.f);
		}
		if (SystemContext::keyBoard.key_is_held(GLFW_KEY_LEFT_SHIFT)) {
			camera_transform->translateLocal(0.f, TimeManager::deltaTime, 0.f);
		}
		if (SystemContext::keyBoard.key_is_held(GLFW_KEY_LEFT_CONTROL)) {
			camera_transform->translateLocal(0.f, -TimeManager::deltaTime, 0.f);
		}

		if (SystemContext::keyBoard.key_is_held(KeyLeft)) {
			camera_transform->rotate(false, true, false, -TimeManager::deltaTime * 30.0f);
		}
		else if (SystemContext::keyBoard.key_is_held(KeyRight)) {
			camera_transform->rotate(false, true, false, TimeManager::deltaTime * 30.0f);
		}
		else if (SystemContext::keyBoard.key_is_held(KeyUp)) {
			camera_transform->rotate(true, false, false, -TimeManager::deltaTime * 30.0f);
		}
		else if (SystemContext::keyBoard.key_is_held(KeyDown)) {
			camera_transform->rotate(true, false, false, TimeManager::deltaTime * 30.0f);
		}

	}
}

void EngineCore::render() {
	renderer->clear(0.f, 0.f, 0.f);
	
	//texManager->get(1)->bind();

	spriteRenderer->render(renderer);

	meshRenderer->render(renderer);

	texManager->get(2)->bind();
	uiRenderer->render(renderer);

	//// temporary text test
	textBuilder.add_int(timeManager->get_current_FPS());
	textManager->add_text_to_buffer(textBuilder.data(), 0.2f, glm::vec3(-SystemContext::screen.ratio, 0.8f, 0.f), timeManager->getRainbowColor(), "Verdana");
	textBuilder.reset();
	//left button
	if (SystemContext::mouse.lb_is_held()) {
		textManager->add_text_to_buffer("left is held", 0.2f, glm::vec3(-1.5f, -0.8f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	else if (SystemContext::mouse.lb_is_idle()) {
		textManager->add_text_to_buffer("left is idle", 0.2f, glm::vec3(-1.5f, -0.8f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	//right button
	if (SystemContext::mouse.rb_is_held()) {
		textManager->add_text_to_buffer("right is held", 0.2f, glm::vec3(-1.5f, -0.6f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	else if (SystemContext::mouse.rb_is_idle()) {
		textManager->add_text_to_buffer("right is idle", 0.2f, glm::vec3(-1.5f, -0.6f, 0.f), glm::vec4(0.9f, 0.9f, 0.0f, 1.f), "Verdana");
	}
	
	
	textManager->render_buffered_text(renderer);
	////

	renderer->present();
}

void EngineCore::input_endFrame() {
	SystemContext::mouse.lb_prev = SystemContext::mouse.lb_curr;
	SystemContext::mouse.rb_prev = SystemContext::mouse.rb_curr;
	SystemContext::mouse.wheel_offset = 0;

	for (int i = 0; i <= GLFW_KEY_LAST; i++) {
		SystemContext::keyBoard.keyStatesPrev[i] = SystemContext::keyBoard.keyStatesCurr[i];
	}
}

void EngineCore::init_component_systems() {
	compSysCore = ES::ComponentSystemsCore::get_Instance();
	compSysCore->register_system<Engine::Component::Transform, ES::TransformSystem>();
	compSysCore->register_system<Engine::Component::SpriteComponent, ES::SpriteComponentSystem>();
	compSysCore->register_system<Engine::Component::MeshComponent, ES::MeshComponentSystem>();
	compSysCore->register_system<Engine::Component::Camera, ES::CameraSystem>();
}

void EngineCore::init_UI_component_systems() {
	uiCompSysCore = ES::UI_ComponentSystemsCore::get_Instance();
	uiCompSysCore->register_system<Engine::UI::Canvas, ES::CanvasSystem>();
	uiCompSysCore->register_system<Engine::UI::UI_Transform, ES::UI_TransformSystem>();
	uiCompSysCore->register_system<Engine::UI::Image, ES::ImageSystem>();
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
