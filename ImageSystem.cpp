#include "ImageSystem.h"

void ImageSystem::add_component(Engine::UI::Image&& comp, uint32_t obj_id) {
	images.emplace(obj_id, comp);
}

Engine::UI::Image* ImageSystem::get_component(uint32_t obj_id) {
	auto it = images.find(obj_id);
	if (it != images.end()) {
		return &it->second;
	}
	else return nullptr;
}

bool ImageSystem::remove_component(uint32_t obj_id) {
	return images.erase(obj_id);
}

void ImageSystem::update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) {

}

void ImageSystem::update() {

}
