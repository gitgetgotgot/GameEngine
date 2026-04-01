#include "ObjectManager.h"

void Engine::Object::ObjectManager::DestroyObject(uint32_t obj_id) {
	Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(obj_id);
}

void Engine::Object::ObjectManager::DestroyObject(object_ptr& object) {
	Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(object.get_id());
}

void Engine::Object::ObjectManager::Update() {
	if (Engine::Systems::DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : Engine::Systems::DestroyedObjects::destroyed_objects) {
			//remove script (just one for now)
			scripts.remove(obj_id);
			//ADD OnDestroy here...
			std::cout << "\t---Script (id = " << obj_id << ") removed---" << std::endl;
			scripts_with_Update.clear(); //clear all for now
			//remove object
			objects.remove(obj_id);
			std::cout << "\t---Object (id = " << obj_id << ") removed---" << std::endl;
		}
		Engine::Systems::DestroyedObjects::destroyed_objects.clear();
	}
	for (auto& script : scripts_with_Update) {
		script->Update();
	}
	for (auto& script : scripts_with_LateUpdate) {
		script->LateUpdate();
	}
}

Engine::Object::Object* Engine::Object::ObjectManager::get_object(uint32_t id) {
	return objects.get(id);
}
