#include <Objects/ComponentSystems/CameraManager.h>
#include <SystemModule/InputHandler.h>

void CameraManager::update_active_camera() {
	if (active_camera) {
		Engine::Component::Transform& tr = *active_camera_transform;
		active_camera->viewMatrix = invertOrthonormal(tr.worldMatrix);
		if (WindowResizeAccess::window_is_resized) {
			WindowResizeAccess::window_is_resized = false;
			std::cout << "Window is resized, Camera updated" << std::endl;
			active_camera->set_perspective(0.1f, 100.0f);
		}
	}
}

void CameraManager::set_active_camera(Engine::Object::object_ptr& camera_object) {
	active_camera = camera_object->get_component<Engine::Component::Camera>();
	active_camera_transform = camera_object->get_component<Engine::Component::Transform>();
}

Engine::Component::Camera& CameraManager::get_active_camera() {
	if (active_camera) return *active_camera;
	else return defaultCamera;
}

void CameraManager::init() {
	defaultCamera.FOV = 45.0f;
	defaultCamera.set_perspective(0.1f, 100.0f);
	update_active_camera();
}

glm::mat4 CameraManager::invertOrthonormal(const glm::mat4& m) {
	glm::mat3 R = glm::mat3(m);
	glm::vec3 t = glm::vec3(m[3]);

	glm::mat3 Rinv = glm::transpose(R);

	glm::vec3 tinv = -Rinv * t;

	glm::mat4 result(1.0f);
	result[0] = glm::vec4(Rinv[0], 0.0f);
	result[1] = glm::vec4(Rinv[1], 0.0f);
	result[2] = glm::vec4(Rinv[2], 0.0f);
	result[3] = glm::vec4(tinv, 1.0f);

	return result;
}
