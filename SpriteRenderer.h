#pragma once
#include <vector>
#include "SpriteManager.h"

struct SpriteUpdater {
	inline static std::vector<uint32_t> dirtySprites;
	inline static std::vector<uint32_t> addedSprites;
	inline static std::vector<uint32_t> deletedSprites;
private:
	SpriteUpdater() {}
	~SpriteUpdater() {}
};

class SpriteRendererSystem;
class SpriteRenderManager;

namespace Engine {
	namespace Component {
		struct SpriteRenderer {
			friend class SpriteRendererSystem;
			friend class SpriteRenderManager;
			Sprite sprite;
			bool flipX = false;
			bool isActive = true;
		private:
			uint32_t obj_id;
		};
	}
}

