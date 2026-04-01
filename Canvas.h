#pragma once
#include <cstdint>
#include <vector>
#include <typeindex>
#include "UI_CompSystemsCore.h"
#include "UI_TransformSystem.h"

class UI_RenderManager;

namespace Engine::UI {
	struct Canvas {
		friend class UI_RenderManager;
		Canvas() {}
		template<typename T>
		void attach_ui_component(uint32_t obj_id);
		void detach_ui_component(uint32_t obj_id);
		bool isEnabled = true;
	private:
		void update_ui_data_buffer(std::vector<Engine::Systems::UI_InstanceData>& ui_data);
		// vector here is essential for UI order in Canvas
		std::vector<std::pair<uint32_t, Engine::Systems::UI_ISystem*>> comp_objects;
	};
	template<typename T>
	inline void Canvas::attach_ui_component(uint32_t obj_id) {
		comp_objects.emplace_back(obj_id, Engine::Systems::UI_ComponentSystemsCore::get_Instance()->get_system_interface<T>());
	}
}
