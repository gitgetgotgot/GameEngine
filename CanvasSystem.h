#pragma once
#include "UI_CompSystemsCore.h"
#include "Canvas.h"

class UI_RenderManager;

class CanvasSystem : public UI_IComponentSystem<Engine::UI::Canvas> {
	friend class UI_RenderManager;
public:
	void add_component(Engine::UI::Canvas&& comp, uint32_t obj_id) override;
	Engine::UI::Canvas* get_component(uint32_t obj_id) override;
	bool remove_component(uint32_t obj_id) override;
	void update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) override;
	void update() override;
private:
	std::unordered_map<uint32_t, Engine::UI::Canvas> canvases;
};