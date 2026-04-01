#include "Canvas.h"

void Engine::UI::Canvas::detach_ui_component(uint32_t obj_id) {
	// find through iteration by obj_id
	int size = comp_objects.size();
	for (int i = 0; i < size; i++) {
		if (comp_objects[i].first == obj_id) {
			comp_objects.erase(comp_objects.begin() + i);
			return;
		}
	}
}

void Engine::UI::Canvas::update_ui_data_buffer(std::vector<Engine::Systems::UI_InstanceData>& ui_data) {
	// for each attached ui object get it's system and it's obj_id, then update buffer
	for (auto& comp : comp_objects) {
		comp.second->update_ui_buffer_data(ui_data, comp.first);
	}
}
