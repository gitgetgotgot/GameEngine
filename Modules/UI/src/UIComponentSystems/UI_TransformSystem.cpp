#include <UI/UIComponentSystems/UI_TransformSystem.h>
#include <Utility/DestroyedObjects.h>

Engine::Systems::UI_TransformSystem::UI_TransformSystem() {}

void Engine::Systems::UI_TransformSystem::add_component(Engine::UI::UI_Transform&& comp, uint32_t obj_id) {
	comp.obj_id = obj_id;
	transforms.add(std::move(comp), obj_id);
}

Engine::UI::UI_Transform* Engine::Systems::UI_TransformSystem::get_component(uint32_t obj_id) {
	return transforms.get(obj_id);
}

bool Engine::Systems::UI_TransformSystem::remove_component(uint32_t obj_id) {
	return transforms.remove(obj_id);
}

void Engine::Systems::UI_TransformSystem::update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) {}

void Engine::Systems::UI_TransformSystem::update() {
	if (DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : DestroyedObjects::destroyed_objects) {
			if (remove_component(obj_id))
				std::cout << "\tComponent UI_Transform (id = " << obj_id << ") removed" << std::endl;
		}
	}
	if (UI_TransformUpdater::dirtyTransforms.size()) {
		for (auto& id : UI_TransformUpdater::dirtyTransforms) {
			Engine::UI::UI_Transform* tr = transforms.get(id);
			if (tr) tr->update_world_matrix();
		}
		UI_TransformUpdater::dirtyTransforms.clear();
	}
}