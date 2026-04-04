#pragma once
#include "SDF_Text.h"

namespace Engine::Systems {
	class SDF_TextSystem : public UI_IComponentSystem<Engine::UI::SDF_Text> {
	public:
		SDF_TextSystem();
		void add_component(Engine::UI::SDF_Text&& comp, uint32_t obj_id) override;
		Engine::UI::SDF_Text* get_component(uint32_t obj_id) override;
		bool remove_component(uint32_t obj_id) override;
		void update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) override;
		void update() override;
	private:
		std::unordered_map<uint32_t, Engine::UI::SDF_Text> texts;
	};
}