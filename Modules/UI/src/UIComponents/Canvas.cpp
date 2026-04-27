#include <UI/UIComponents/Canvas.h>

void Engine::UI::Canvas::update_ui_data_buffer(
	std::vector<Engine::Systems::UI_InstanceData>& ui_data,
	std::vector<Engine::Systems::UI_InstanceData>& ui_text_data
) {
	// for each attached ui object get it's system and it's obj_id, then update buffer
	if (obj_transform) {
		std::vector<UI::UI_Component_Ptr<UI::UI_Transform>>& children = obj_transform->get_children();
		uint32_t component_obj_id;
		for (auto& c : children) {
			update_child(c, ui_data, ui_text_data);
		}
	}

}

void Engine::UI::Canvas::update_child(
	UI_Component_Ptr<UI_Transform>& child_transform,
	std::vector<Engine::Systems::UI_InstanceData>& ui_data,
	std::vector<Engine::Systems::UI_InstanceData>& ui_text_data
) {
	uint32_t component_obj_id = child_transform.get_object_id();
	auto it = Systems::UI_Components_Data_Access::comp_objects.find(component_obj_id);
	if (it != Systems::UI_Components_Data_Access::comp_objects.end()) {
		if (it->second->type == Systems::UI_System_Type::isNotText)
			it->second->update_ui_buffer_data(ui_data, component_obj_id);
		else
			it->second->update_ui_buffer_data(ui_text_data, component_obj_id);
	}
	std::vector<UI::UI_Component_Ptr<UI::UI_Transform>>& children = child_transform->get_children();
	for (auto& c : children)
		update_child(c, ui_data, ui_text_data);
}
