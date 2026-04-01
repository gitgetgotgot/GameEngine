#include "PlayerController.h"
#include "SystemContext.h"
#include "TimeManager.h"

void PlayerController::OnConstruct() {
	transform = object->add_component<Engine::Component::Transform>();
	transform->translateLocal(0, 0, 2.f);
	//sc = object->add_component<Engine::Component::SpriteComponent>();
	
	/*mc = object->add_component<Engine::Component::MeshComponent>();
	Mesh& mesh = MeshManager::get_Instance()->get_mesh(4);
	mc->mesh_id = mesh.id;*/
	std::cout << "Player controller constructed" << std::endl;
}

void PlayerController::Update() {
	if (SystemContext::keyBoard.key_is_pressed(KeyP)) {
		canControl = !canControl;
	}

	if (!canControl) return;

	if (SystemContext::keyBoard.key_is_held(KeyW)) {
		transform->translateLocal(0.f, 0.f, TimeManager::deltaTime);
	}
	if (SystemContext::keyBoard.key_is_held(KeyA)) {
		transform->translateLocal(-TimeManager::deltaTime, 0.f, 0.f);
	}
	if (SystemContext::keyBoard.key_is_held(KeyS)) {
		transform->translateLocal(0.f, 0.f, -TimeManager::deltaTime);
	}
	if (SystemContext::keyBoard.key_is_held(KeyD)) {
		transform->translateLocal(TimeManager::deltaTime, 0.f, 0.f);
	}
	if (SystemContext::keyBoard.key_is_held(GLFW_KEY_LEFT_SHIFT)) {
		transform->translateLocal(0.f, TimeManager::deltaTime, 0.f);
	}
	if (SystemContext::keyBoard.key_is_held(GLFW_KEY_LEFT_CONTROL)) {
		transform->translateLocal(0.f, -TimeManager::deltaTime, 0.f);
	}

	if (SystemContext::keyBoard.key_is_held(KeyLeft)) {
		transform->rotate(false, true, false, -TimeManager::deltaTime * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.key_is_held(KeyRight)) {
		transform->rotate(false, true, false, TimeManager::deltaTime * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.key_is_held(KeyUp)) {
		transform->rotate(true, false, false, -TimeManager::deltaTime * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.key_is_held(KeyDown)) {
		transform->rotate(true, false, false, TimeManager::deltaTime * cameraRotateSpeed);
	}
	
	if (SystemContext::mouse.lb_is_pressed()) {
		image->flipX(!image->flip_X);
	}
	else if (SystemContext::mouse.rb_is_pressed()) {
		image->flipY(!image->flip_Y);
	}

	if (SystemContext::keyBoard.key_is_pressed(KeyDelete)) {
		object->Destroy();
	}
}

void PlayerController::set_test_image(EUI::UI_Component_Ptr<EUI::Image>& img_ptr) {
	this->image = img_ptr;
}
