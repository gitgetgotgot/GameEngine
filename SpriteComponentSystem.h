#pragma once
#include "CompSystemsCore.h"
#include "SpriteComponent.h"

class SpriteRenderManager;

namespace Engine::Systems {
	class SpriteComponentSystem : public IComponentSystem<Engine::Component::SpriteComponent> {
		friend class SpriteRenderManager;
	public:
		void add_component(Engine::Component::SpriteComponent&& comp, uint32_t obj_id) override;
		Engine::Component::SpriteComponent* get_component(uint32_t obj_id) override;
		bool remove_component(uint32_t obj_id) override;
		void update() override;
	private:
		sparse_set<Engine::Component::SpriteComponent> spriteRenderers;
	};
}