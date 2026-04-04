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

namespace Engine::Systems {
	class SpriteComponentSystem;
}

class SpriteRenderManager;

namespace Engine::Component {
	class SpriteComponent {
	public:
		friend class Engine::Systems::SpriteComponentSystem;
		friend class SpriteRenderManager;
		Sprite sprite;
		bool flipX = false;
		bool isActive = true;
	private:
		uint32_t obj_id;
	};
}