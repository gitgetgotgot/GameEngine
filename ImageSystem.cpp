#include "ImageSystem.h"

void Engine::Systems::ImageSystem::add_component(Engine::UI::Image&& comp, uint32_t obj_id) {
	comp.obj_id = obj_id;
	comp.transform_ptr = Engine::UI::UI_Component_Ptr<Engine::UI::UI_Transform>(obj_id);
	images.emplace(obj_id, comp);
	UI_Components_Data_Access::comp_objects.emplace(obj_id, static_cast<UI_ISystem*>(this));
}

Engine::UI::Image* Engine::Systems::ImageSystem::get_component(uint32_t obj_id) {
	auto it = images.find(obj_id);
	if (it != images.end()) {
		return &it->second;
	}
	else return nullptr;
}

bool Engine::Systems::ImageSystem::remove_component(uint32_t obj_id) {
	return images.erase(obj_id);
}

void Engine::Systems::ImageSystem::update_ui_buffer_data(std::vector<Engine::Systems::UI_InstanceData>& ui_data, uint32_t obj_id) {
	Engine::UI::Image& img = images[obj_id];
	if (!img.isActive) return;
	img.data.modelMatrix = img.transform_ptr->get_final_ui_matrix(); ///SHOULD CHECK IF THIS IS REALLY NEEDED
	ui_data.emplace_back(img.data);
}

void Engine::Systems::ImageSystem::update() {
	if (ImageUpdater::dirtyImages.size() != 0) {
		for (auto& image_obj_id : ImageUpdater::dirtyImages) {
			images[image_obj_id]._internal_update_ui_data();
		}
		ImageUpdater::dirtyImages.clear();
	}
}
