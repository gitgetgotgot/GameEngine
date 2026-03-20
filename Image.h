#pragma once
#include "SpriteManager.h"

namespace Engine {
	namespace UI {
		struct Image {
			friend class SpriteRendererSystem;
			friend class SpriteRenderManager;
			Sprite sprite;
			glm::vec4 color;
			bool flipX = false;
			bool isActive = true;
		private:
			uint32_t obj_id;
		};
	}
}
