#pragma once
#include <UI/UIComponents/Image.h>

namespace Engine::Systems {
	class ImageSystem : public UI_IComponentSystem<Engine::UI::Image> {
	public:
		void add_component(Engine::UI::Image&& comp, uint32_t obj_id) override;
		Engine::UI::Image* get_component(uint32_t obj_id) override;
		bool remove_component(uint32_t obj_id) override;
		void update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) override;
		void update() override;
	private:
		std::unordered_map<uint32_t, Engine::UI::Image> images;
	};
}