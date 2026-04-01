#include "MeshComponentSystem.h"

void Engine::Systems::MeshComponentSystem::add_component(Engine::Component::MeshComponent&& comp, uint32_t obj_id) {
	comp.obj_id = obj_id;
	meshComponents.add(std::move(comp), obj_id);
	MeshUpdater::addedMeshes.emplace_back(obj_id);
}

Engine::Component::MeshComponent* Engine::Systems::MeshComponentSystem::get_component(uint32_t obj_id) {
	return meshComponents.get(obj_id);
}

bool Engine::Systems::MeshComponentSystem::remove_component(uint32_t obj_id) {
	return meshComponents.remove(obj_id);
}

void Engine::Systems::MeshComponentSystem::update() {
	if (DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : DestroyedObjects::destroyed_objects) {
			Engine::Component::MeshComponent* mc = get_component(obj_id);
			if (mc) {
				MeshUpdater::deletedMeshes.emplace_back(obj_id, mc->mesh_id);
				remove_component(obj_id);
				std::cout << "\tComponent MeshComponent (id = " << obj_id << ") removed" << std::endl;
			}
		}
	}
	if (MeshUpdater::dirtyMeshes.size()) {
		for (auto& id : MeshUpdater::dirtyMeshes) {

		}
		MeshUpdater::dirtyMeshes.clear();
	}
}
