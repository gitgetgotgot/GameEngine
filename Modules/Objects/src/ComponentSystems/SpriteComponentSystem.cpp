#include <Objects/ComponentSystems/SpriteComponentSystem.h>
#include <Utility/DestroyedObjects.h>

void Engine::Systems::SpriteComponentSystem::add_component(Engine::Component::SpriteComponent&& comp, uint32_t obj_id) {
	comp.obj_id = obj_id;
	spriteRenderers.add(std::move(comp), obj_id);
	SpriteUpdater::addedSprites.emplace_back(obj_id);
}

Engine::Component::SpriteComponent* Engine::Systems::SpriteComponentSystem::get_component(uint32_t obj_id) {
	return spriteRenderers.get(obj_id);
}

bool Engine::Systems::SpriteComponentSystem::remove_component(uint32_t obj_id) {
	return spriteRenderers.remove(obj_id);
}

void Engine::Systems::SpriteComponentSystem::update() {
	if (DestroyedObjects::destroyed_objects.size()) {
		for (auto& obj_id : DestroyedObjects::destroyed_objects) {
			if (remove_component(obj_id)) {
				SpriteUpdater::deletedSprites.emplace_back(obj_id);
				std::cout << "\tComponent SpriteComponent (id = " << obj_id << ") removed" << std::endl;
			}
		}
	}
	if (SpriteUpdater::dirtySprites.size()) {
		for (auto& id : SpriteUpdater::dirtySprites) {
			Engine::Component::SpriteComponent* sr = spriteRenderers.get(id);

		}
		SpriteUpdater::dirtySprites.clear();
	}
}
