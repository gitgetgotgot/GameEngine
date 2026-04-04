#include "ButtonSystem.h"

void Engine::Systems::ButtonSystem::add_component(Engine::UI::Button&& comp, uint32_t obj_id) {
	comp.obj_id = obj_id;
	comp.transform_ptr = Engine::UI::UI_Component_Ptr<Engine::UI::UI_Transform>(obj_id);
	buttons.emplace(obj_id, comp);
	UI_Components_Data_Access::comp_objects.emplace(obj_id, static_cast<UI_ISystem*>(this));
}

Engine::UI::Button* Engine::Systems::ButtonSystem::get_component(uint32_t obj_id) {
	auto it = buttons.find(obj_id);
	if (it != buttons.end()) {
		return &it->second;
	}
	else return nullptr;
}

bool Engine::Systems::ButtonSystem::remove_component(uint32_t obj_id) {
	return buttons.erase(obj_id);
}

void Engine::Systems::ButtonSystem::update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) {
	Engine::UI::Button& button = buttons[obj_id];
	if (!button.isActive) return;
	button.data.modelMatrix = button.transform_ptr->get_final_ui_matrix(); ///SHOULD CHECK IF THIS IS REALLY NEEDED
	ui_data.emplace_back(button.data);
}

void Engine::Systems::ButtonSystem::update() {
	if (ButtonUpdater::dirtyButtons.size() != 0) {
		for (auto& image_obj_id : ButtonUpdater::dirtyButtons) {
			buttons[image_obj_id]._internal_update_ui_data();
		}
		ButtonUpdater::dirtyButtons.clear();
	}
	for (auto& button : buttons) {
		button.second._internal_update();
	}
}
