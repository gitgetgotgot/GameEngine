#pragma once
#include "UI_CompSystemsCore.h"
#include "UI_Transform.h"

class UI_TransformSystem : public UI_IComponentSystem<Engine::UI::UI_Transform> {
public:
	friend class SpriteRenderManager;
	friend class MeshRenderManager;
	UI_TransformSystem();
	void add_component(Engine::UI::UI_Transform&& comp, uint32_t obj_id) override;
	Engine::UI::UI_Transform* get_component(uint32_t obj_id) override;
	bool remove_component(uint32_t obj_id) override;
	void update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) override;
	void update() override;
private:
	sparse_set<Engine::UI::UI_Transform> transforms;
};