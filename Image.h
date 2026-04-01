#pragma once
#include "UI_Transform.h"
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

namespace Engine {
	namespace UI {
		struct Image {
			friend class Engine::Systems::ImageSystem;
			Sprite* sprite;
			glm::vec4 color;
			bool flip_X = false;
			bool flip_Y = false;
			bool isActive = true;
			void set_sprite(Sprite* sprite);
			void set_color(glm::vec4 color);
			void flipX(bool is_flipped);
			void flipY(bool is_flipped);
		private:
			uint32_t obj_id;
			void mark_dirty();
			void update_model_matrix();
			void update_ui_data();
			UI_Component_Ptr<Engine::UI::UI_Transform> transform_ptr;
			Engine::Systems::UI_InstanceData data{};
			bool isDirty = false;
		};
	}
}
