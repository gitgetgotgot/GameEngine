#pragma once
#include "UI_Component.h"
#include "SpriteManager.h"

struct ButtonUpdater {
	inline static std::vector<uint32_t> dirtyButtons;
private:
	ButtonUpdater() {}
	~ButtonUpdater() {}
};

namespace Engine::Systems {
	class ButtonSystem;
}

namespace Engine::UI {
	class Button : public UI_Component {
		friend class Engine::Systems::ButtonSystem;
	public:
		Sprite* sprite;
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		void set_callback(void(*staticCallbackWrapper)(void*), void* callback_obj);
		void set_sprite(Sprite& sprite);
		void set_color(glm::vec4 color);
		void _internal_update_ui_data() override;
		void _internal_update();
	private:
		void mark_dirty() override;
		void on_click();
		Engine::Systems::UI_InstanceData data{};
		bool isPointed = false;
		void (*callback)(void*) = nullptr;
		void* callback_object = nullptr;
	};
}