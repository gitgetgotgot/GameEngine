#pragma once
#include <Objects/Components/Camera.h>
#include <Objects/Base/Object.h>

class CameraManager {
public:
	static CameraManager* get_Instance() {
		static CameraManager cameraHolder;
		return &cameraHolder;
	}
	void update_active_camera();
	void set_active_camera(Engine::Object::object_ptr& camera_object);
	Engine::Component::Camera& get_active_camera();
	void init();
private:
	glm::mat4 invertOrthonormal(const glm::mat4& m);
	CameraManager() {}
	~CameraManager() {}
	Engine::Component::Camera defaultCamera;
	Engine::Component::Component_Ptr<Engine::Component::Camera> active_camera;
	Engine::Component::Component_Ptr<Engine::Component::Transform> active_camera_transform;
};