#include "SDF_TextSystem.h"

Engine::Systems::SDF_TextSystem::SDF_TextSystem() {
	this->type = UI_System_Type::isText;
}

void Engine::Systems::SDF_TextSystem::add_component(Engine::UI::SDF_Text&& comp, uint32_t obj_id) {
	comp.obj_id = obj_id;
	comp.transform_ptr = Engine::UI::UI_Component_Ptr<Engine::UI::UI_Transform>(obj_id);
	texts.emplace(obj_id, comp);
	UI_Components_Data_Access::comp_objects.emplace(obj_id, static_cast<UI_ISystem*>(this));
}

Engine::UI::SDF_Text* Engine::Systems::SDF_TextSystem::get_component(uint32_t obj_id) {
	auto it = texts.find(obj_id);
	if (it != texts.end()) {
		return &it->second;
	}
	else return nullptr;
}

bool Engine::Systems::SDF_TextSystem::remove_component(uint32_t obj_id) {
	return texts.erase(obj_id);
}

void Engine::Systems::SDF_TextSystem::update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) {
	Engine::UI::SDF_Text& text = texts[obj_id];
	if (!text.isActive) return;
	for (auto& glyph : text.data) {
		UI_InstanceData data = glyph;
		data.modelMatrix = text.transform_ptr->get_world_matrix() * glyph.modelMatrix;
		ui_data.emplace_back(data);
	}
}

void Engine::Systems::SDF_TextSystem::update() {
	if (SDF_TextUpdater::dirtyTexts.size() != 0) {
		for (auto& image_obj_id : SDF_TextUpdater::dirtyTexts) {
			texts[image_obj_id]._internal_update_ui_data();
		}
		SDF_TextUpdater::dirtyTexts.clear();
	}
}
