#include "MeshRendererSystem.h"

void MeshRendererSystem::add_component(Engine::Component::MeshRenderer&& comp, uint32_t obj_id) {
	Engine::Component::MeshRenderer* mr = meshRenderers.add(std::move(comp), obj_id);
	mr->obj_id = obj_id; //should remake
	MeshUpdater::addedMeshes.emplace_back(obj_id);
}

Engine::Component::MeshRenderer* MeshRendererSystem::get_component(uint32_t obj_id) {
	return meshRenderers.get(obj_id);
}

bool MeshRendererSystem::remove_component(uint32_t obj_id) {
	return meshRenderers.remove(obj_id);
}

void MeshRendererSystem::update() {
	if (DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : DestroyedObjects::destroyed_objects) {
			Engine::Component::MeshRenderer* mr = get_component(obj_id);
			if (mr) {
				MeshUpdater::deletedMeshes.emplace_back(obj_id, mr->mesh->id);
				remove_component(obj_id);
				std::cout << "\tComponent MeshRenderer (id = " << obj_id << ") removed" << std::endl;
			}
		}
	}
	if (MeshUpdater::dirtyMeshes.size()) {
		for (auto& id : MeshUpdater::dirtyMeshes) {

		}
		MeshUpdater::dirtyMeshes.clear();
	}
}
