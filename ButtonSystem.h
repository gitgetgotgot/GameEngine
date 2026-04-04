#pragma once
#include "Button.h"

namespace Engine::Systems {
	class ButtonSystem : public UI_IComponentSystem<Engine::UI::Button> {
	public:
		void add_component(Engine::UI::Button&& comp, uint32_t obj_id) override;
		Engine::UI::Button* get_component(uint32_t obj_id) override;
		bool remove_component(uint32_t obj_id) override;
		void update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) override;
		void update() override;
	private:
		std::unordered_map<uint32_t, Engine::UI::Button> buttons;
	};
}