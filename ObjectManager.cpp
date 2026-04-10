#include "ObjectManager.h"

void Engine::Object::ObjectManager::DestroyObject(uint32_t obj_id) {
	Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(obj_id);
}

void Engine::Object::ObjectManager::DestroyObject(object_ptr& object) {
	Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(object.get_id());
}

void Engine::Object::ObjectManager::_internal_init() {
	object_ptr::object_manager_ptr = get_Instance();
}

void Engine::Object::ObjectManager::_internal_update() {
	if (Engine::Systems::DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : Engine::Systems::DestroyedObjects::destroyed_objects) {
			if (objects.remove(obj_id)) {
				free_ids.emplace_back(obj_id);
				std::cout << "[ObjectManager]: Object(" << obj_id << ") removed" << std::endl;
			}
		}

	}
}

Engine::Object::Object* Engine::Object::ObjectManager::get_object(uint32_t id) {
	return objects.get(id);
}
