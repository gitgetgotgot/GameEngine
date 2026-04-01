#include "CanvasSystem.h"

void Engine::Systems::CanvasSystem::add_component(Engine::UI::Canvas&& comp, uint32_t obj_id) {
	canvases.emplace(obj_id, comp);
}

Engine::UI::Canvas* Engine::Systems::CanvasSystem::get_component(uint32_t obj_id) {
	auto it = canvases.find(obj_id);
	if (it != canvases.end()) {
		return &it->second;
	}
	else return nullptr;
}

bool Engine::Systems::CanvasSystem::remove_component(uint32_t obj_id) {
	return canvases.erase(obj_id);
}

void Engine::Systems::CanvasSystem::update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) {}

void Engine::Systems::CanvasSystem::update() {}
