#pragma once
#include <cstdint>
#include "UI_Transform.h"

namespace Engine::UI {
	class UI_Component {
	public:
		bool isActive = true;
		virtual void _internal_update_ui_data() {}
		virtual ~UI_Component() {}
	protected:
		virtual void mark_dirty() {};
		uint32_t obj_id;
		bool isDirty = false;
		UI_Component_Ptr<Engine::UI::UI_Transform> transform_ptr;
	};
}