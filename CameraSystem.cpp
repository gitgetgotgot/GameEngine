#include "CameraSystem.h"

void CameraSystem::add_component(Engine::Component::Camera&& comp, uint32_t obj_id) {
	cameras.emplace(obj_id, comp);
}

Engine::Component::Camera* CameraSystem::get_component(uint32_t obj_id) {
	auto it = cameras.find(obj_id);
	if (it != cameras.end()) {
		return &it->second;
	}
	else return nullptr;
}

bool CameraSystem::remove_component(uint32_t obj_id) {
	return cameras.erase(obj_id);
}

void CameraSystem::update() {}
