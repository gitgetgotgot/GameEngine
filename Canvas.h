#pragma once
#include <cstdint>
#include <vector>
#include <typeindex>

class UI_RenderManager;

namespace Engine {
	namespace UI {
		struct Canvas {
			friend class UI_RenderManager;
			Canvas() {}
			template<typename T>
			void attach_ui_component(uint32_t obj_id);
			void detach_ui_component(uint32_t obj_id);
			bool isEnabled = true;
		private:
			std::vector<std::pair<uint32_t, std::type_index>> comp_objects;
		};
		template<typename T>
		inline void Canvas::attach_ui_component(uint32_t obj_id) {
			comp_objects.emplace_back(obj_id, std::type_index(typeid(T)));
		}
	}
}
