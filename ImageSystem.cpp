#include "ImageSystem.h"

void Engine::Systems::ImageSystem::add_component(Engine::UI::Image&& comp, uint32_t obj_id) {
	images.emplace(obj_id, comp);
	Engine::UI::Image& image = images[obj_id];
	image.obj_id = obj_id;
	image.transform_ptr = Engine::UI::UI_Component_Ptr<Engine::UI::UI_Transform>(obj_id);
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
	img.update_model_matrix(); ///SHOULD CHECK IF THIS IS REALLY NEEDED
	ui_data.emplace_back(img.data);
}

void Engine::Systems::ImageSystem::update() {
	if (ImageUpdater::dirtyImages.size() != 0) {
		for (auto& image_obj_id : ImageUpdater::dirtyImages) {
			images[image_obj_id].update_ui_data();
		}
		ImageUpdater::dirtyImages.clear();
	}
}
