#pragma once
#include "CompSystemsCore.h"
#include "MeshComponent.h"

class MeshRenderManager;

namespace Engine::Systems {
	class MeshComponentSystem : public IComponentSystem<Engine::Component::MeshComponent> {
		friend class MeshRenderManager;
	public:
		void add_component(Engine::Component::MeshComponent&& comp, uint32_t obj_id) override;
		Engine::Component::MeshComponent* get_component(uint32_t obj_id) override;
		bool remove_component(uint32_t obj_id) override;
		void update() override;
	private:
		sparse_set<Engine::Component::MeshComponent> meshComponents;
	};
}
