#pragma once
#include "CompSystemsCore.h"
#include "Camera.h"

class CameraSystem : public IComponentSystem<Engine::Component::Camera> {
public:
	friend class SpriteRenderManager;
	friend class MeshRenderManager;
	void add_component(Engine::Component::Camera&& comp, uint32_t obj_id) override;
	Engine::Component::Camera* get_component(uint32_t obj_id) override;
	bool remove_component(uint32_t obj_id) override;
	void update() override;
private:
	std::unordered_map<uint32_t, Engine::Component::Camera> cameras;
};