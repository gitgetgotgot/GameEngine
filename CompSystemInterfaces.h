#pragma once
#include "SparseSet.h"
#include "SpriteManager.h"
#include <unordered_map>
#include <typeindex>
#include <memory>

// struct that is used in object manager and different component systems
// (maybe this method should be reworked later to exclude individual delete in all systems)
struct DestroyedObjects {
	inline static std::vector<uint32_t> destroyed_objects;
};

#include <iostream> // temporary for logs in systems

class ISystem {
public:
	virtual ~ISystem() {}
	virtual void update() = 0;
};

template<typename Comp>
class IComponentSystem : public ISystem {
public:
	virtual void add_component(Comp&& comp, uint32_t obj_id) = 0;
	virtual Comp * get_component(uint32_t obj_id) = 0;
	virtual bool remove_component(uint32_t obj_id) = 0;
};

struct UI_InstanceData {
	UI_InstanceData() {}
	UI_InstanceData(Sprite& sprite, glm::mat4& mat) {
		modelMatrix = mat;
		UV[0] = sprite.UV[0]; UV[1] = sprite.UV[1]; UV[2] = sprite.UV[2]; UV[3] = sprite.UV[3];
	}
	glm::mat4 modelMatrix{};
	glm::vec2 UV[4]{};
	glm::vec4 color{};
	uint64_t handle{};
};

template<typename UI_Comp>
class UI_IComponentSystem : public ISystem {
public:
	virtual void add_component(UI_Comp&& comp, uint32_t obj_id) = 0;
	virtual UI_Comp* get_component(uint32_t obj_id) = 0;
	virtual bool remove_component(uint32_t obj_id) = 0;
	virtual void update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) = 0;
};
