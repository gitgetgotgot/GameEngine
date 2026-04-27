#pragma once
#include <cstdint>
#include <vector>
#include <typeindex>
#include <UI/UIComponentSystems/UI_CompSystemsCore.h>
#include <UI/UIComponentSystems/UI_TransformSystem.h>

class UI_RenderManager;

namespace Engine::Systems {
	class CanvasSystem;
}

namespace Engine::UI {
	class Canvas {
		friend class Engine::Systems::CanvasSystem;
	public:
		friend class UI_RenderManager;
		Canvas() {}
		bool isEnabled = true;
	private:
		uint32_t obj_id{};
		UI_Component_Ptr<UI_Transform> obj_transform;
		void update_ui_data_buffer(
			std::vector<Engine::Systems::UI_InstanceData>& ui_data,
			std::vector<Engine::Systems::UI_InstanceData>& ui_text_data
		);
		void update_child(
			UI_Component_Ptr<UI_Transform>& child_transform,
			std::vector<Engine::Systems::UI_InstanceData>& ui_data,
			std::vector<Engine::Systems::UI_InstanceData>& ui_text_data
		);
	};
}