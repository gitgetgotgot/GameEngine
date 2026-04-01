#pragma once
#include "Transform.h"

class SpriteRenderManager;
class MeshRenderManager;

namespace Engine::Systems {
	class TransformSystem : public IComponentSystem<Engine::Component::Transform> {
	public:
		friend class SpriteRenderManager;
		friend class MeshRenderManager;
		TransformSystem();
		void add_component(Engine::Component::Transform&& comp, uint32_t obj_id) override;
		Engine::Component::Transform* get_component(uint32_t obj_id) override;
		bool remove_component(uint32_t obj_id) override;
		void update() override;
	private:
		sparse_set<Engine::Component::Transform> transforms;
	};
}