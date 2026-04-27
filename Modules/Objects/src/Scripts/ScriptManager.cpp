#include <Objects/Scripts/ScriptManager.h>
#include <Utility/DestroyedObjects.h>

void Engine::Object::ScriptManager::_internal_init() {}

void Engine::Object::ScriptManager::_internal_update() {
	if (Engine::Systems::DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : Engine::Systems::DestroyedObjects::destroyed_objects) {
			if (scripts.remove(obj_id))
				std::cout << "[ScriptManager]: Script(" << obj_id << ") removed" << std::endl;
		}
		//this is the last place where destroyed objects are checked, so they are cleared here
		Engine::Systems::DestroyedObjects::destroyed_objects.clear();
	}
	auto size = scripts_with_Update.size();
	for (uint32_t i = 0; i < size; i++) {
		auto script = scripts.get(scripts_with_Update[i]);
		if (!script) {
			scripts_with_Update[i] = scripts_with_Update.back();
			scripts_with_Update.pop_back();
			i--;
			size--;
			continue;
		}
		(*script)->Update();
	}
	size = scripts_with_LateUpdate.size();
	for (uint32_t i = 0; i < size; i++) {
		auto script = scripts.get(scripts_with_LateUpdate[i]);
		if (!script) {
			scripts_with_LateUpdate[i] = scripts_with_LateUpdate.back();
			scripts_with_LateUpdate.pop_back();
			i--;
			size--;
			continue;
		}
		(*script)->LateUpdate();
	}
}
