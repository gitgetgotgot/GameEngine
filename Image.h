#pragma once
#include "UI_Component.h"
#include "SpriteManager.h"

struct ImageUpdater {
	inline static std::vector<uint32_t> dirtyImages;
private:
	ImageUpdater() {}
	~ImageUpdater() {}
};

namespace Engine::Systems {
	class ImageSystem;
}

namespace Engine::UI {
	class Image : public UI_Component {
		friend class Engine::Systems::ImageSystem;
	public:
		Sprite* sprite;
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		bool flip_X = false;
		bool flip_Y = false;
		void set_sprite(Sprite* sprite);
		void set_color(glm::vec4 color);
		void flipX(bool is_flipped);
		void flipY(bool is_flipped);
		void _internal_update_ui_data() override;
	private:
		void mark_dirty() override;
		Engine::Systems::UI_InstanceData data{};
	};
}
