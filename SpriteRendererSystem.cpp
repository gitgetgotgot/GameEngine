#include "SpriteRendererSystem.h"

void SpriteRendererSystem::add_component(Engine::Component::SpriteRenderer&& comp, uint32_t obj_id) {
	spriteRenderers.add(std::move(comp), obj_id);
	Engine::Component::SpriteRenderer* sr = spriteRenderers.get(obj_id);
	sr->obj_id = obj_id; //should remake
	SpriteUpdater::addedSprites.emplace_back(obj_id);
}

Engine::Component::SpriteRenderer* SpriteRendererSystem::get_component(uint32_t obj_id) {
	return spriteRenderers.get(obj_id);
}

bool SpriteRendererSystem::remove_component(uint32_t obj_id) {
	return spriteRenderers.remove(obj_id);
}

void SpriteRendererSystem::update() {
	if (DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : DestroyedObjects::destroyed_objects) {
			if (remove_component(obj_id)) {
				SpriteUpdater::deletedSprites.emplace_back(obj_id);
				std::cout << "\tComponent SpriteRenderer (id = " << obj_id << ") removed" << std::endl;
			}
		}
	}
	if (SpriteUpdater::dirtySprites.size()) {
		for (auto& id : SpriteUpdater::dirtySprites) {
			Engine::Component::SpriteRenderer* sr = spriteRenderers.get(id);

		}
		SpriteUpdater::dirtySprites.clear();
	}
}
