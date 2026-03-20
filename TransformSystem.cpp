#include "TransformSystem.h"

TransformSystem::TransformSystem() {

}

void TransformSystem::add_component(Engine::Component::Transform&& comp, uint32_t obj_id) {
	Engine::Component::Transform* tr = transforms.add(std::move(comp), obj_id);
	tr->obj_id = obj_id;
}

Engine::Component::Transform* TransformSystem::get_component(uint32_t obj_id) {
	return transforms.get(obj_id);
}

bool TransformSystem::remove_component(uint32_t obj_id) {
	return transforms.remove(obj_id);
}

void TransformSystem::update() {
	if (DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : DestroyedObjects::destroyed_objects) {
			if(remove_component(obj_id))
				std::cout << "\tComponent Transform (id = " << obj_id << ") removed" << std::endl;
		}
	}
	if (TransformUpdater::dirtyTransforms.size()) {
		for (auto& id : TransformUpdater::dirtyTransforms) {
			Engine::Component::Transform* tr = transforms.get(id);
			if(tr) tr->update_modelMatrix();
		}
		TransformUpdater::dirtyTransforms.clear();
	}
}
