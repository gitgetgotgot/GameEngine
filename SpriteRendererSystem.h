#pragma once
#include "CompSystemsCore.h"
#include "SpriteRenderer.h"

class SpriteRenderManager;

class SpriteRendererSystem : public IComponentSystem<Engine::Component::SpriteRenderer> {
	friend class SpriteRenderManager;
public:
	void add_component(Engine::Component::SpriteRenderer&& comp, uint32_t obj_id) override;
	Engine::Component::SpriteRenderer* get_component(uint32_t obj_id) override;
	bool remove_component(uint32_t obj_id) override;
	void update() override;
private:
	sparse_set<Engine::Component::SpriteRenderer> spriteRenderers;
};