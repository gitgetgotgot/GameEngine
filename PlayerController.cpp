#include "PlayerController.h"
#include "SystemContext.h"
#include "GLFW/glfw3.h"
#include "TimeManager.h"

void PlayerController::OnConstruct() {
	transform = object->add_component<Engine::Component::Transform>();
	transform->translateLocal(0, 0, 2.f);
	//sr = object->add_component<Engine::Component::SpriteRenderer>();
	
	mr = object->add_component<Engine::Component::MeshRenderer>();
	mr->mesh = MeshManager::get_Instance()->get_mesh(1);
	
	canControl = true;
	std::cout << "Player controller constructed" << std::endl;
}

void PlayerController::Update() {
	if (SystemContext::keyBoard.keyStates[GLFW_KEY_P]) {
		SystemContext::keyBoard.keyStates[GLFW_KEY_P] = false;
		canControl = !canControl;
	}

	if (!canControl) return;

	if (SystemContext::keyBoard.keyStates[GLFW_KEY_W]) {
		transform->translateLocal(0.f, 0.f, TimeManager::deltaTime);
	}
	if (SystemContext::keyBoard.keyStates[GLFW_KEY_A]) {
		transform->translateLocal(-TimeManager::deltaTime, 0.f, 0.f);
	}
	if (SystemContext::keyBoard.keyStates[GLFW_KEY_S]) {
		transform->translateLocal(0.f, 0.f, -TimeManager::deltaTime);
	}
	if (SystemContext::keyBoard.keyStates[GLFW_KEY_D]) {
		transform->translateLocal(TimeManager::deltaTime, 0.f, 0.f);
	}
	if (SystemContext::keyBoard.keyStates[GLFW_KEY_LEFT_SHIFT]) {
		transform->translateLocal(0.f, TimeManager::deltaTime, 0.f);
	}
	if (SystemContext::keyBoard.keyStates[GLFW_KEY_LEFT_CONTROL]) {
		transform->translateLocal(0.f, -TimeManager::deltaTime, 0.f);
	}

	if (SystemContext::keyBoard.keyStates[GLFW_KEY_LEFT]) {
		transform->rotate(false, true, false, -TimeManager::deltaTime * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.keyStates[GLFW_KEY_RIGHT]) {
		transform->rotate(false, true, false, TimeManager::deltaTime * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.keyStates[GLFW_KEY_UP]) {
		transform->rotate(true, false, false, -TimeManager::deltaTime * cameraRotateSpeed);
	}
	else if (SystemContext::keyBoard.keyStates[GLFW_KEY_DOWN]) {
		transform->rotate(true, false, false, TimeManager::deltaTime * cameraRotateSpeed);
	}
	
	if (SystemContext::keyBoard.keyStates[GLFW_KEY_1]) {
		transform->set_scale(1.f, 1.f, 1.f);
	}
	else if (SystemContext::keyBoard.keyStates[GLFW_KEY_2]) {
		transform->set_scale(2.f, 2.f, 2.f);
	}

	if (SystemContext::keyBoard.keyStates[GLFW_KEY_DELETE]) {
		SystemContext::keyBoard.keyStates[GLFW_KEY_DELETE] = false;
		//Engine::Object::ObjectManager::get_Instance()->DestroyObject(object);
		object->Destroy();
	}
}
