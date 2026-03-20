#pragma once
#include "CompSystemsCore.h"
#include "MeshRenderer.h"

class MeshRenderManager;

class MeshRendererSystem : public IComponentSystem<Engine::Component::MeshRenderer> {
	friend class MeshRenderManager;
public:
	void add_component(Engine::Component::MeshRenderer&& comp, uint32_t obj_id) override;
	Engine::Component::MeshRenderer* get_component(uint32_t obj_id) override;
	bool remove_component(uint32_t obj_id) override;
	void update() override;
private:
	sparse_set<Engine::Component::MeshRenderer> meshRenderers;
};
