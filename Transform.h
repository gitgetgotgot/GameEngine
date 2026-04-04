#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include "CompSystemsCore.h"

class SpriteRenderManager;
class MeshRenderManager;
class CameraManager;

struct TransformUpdater {
	inline static std::vector<uint32_t> dirtyTransforms;
private:
	TransformUpdater() {}
	~TransformUpdater() {}
};

namespace Engine::Systems {
	class TransformSystem;
}

namespace Engine::Component {
	class Transform {
	public:
		friend class Engine::Systems::TransformSystem;
		friend class SpriteRenderManager;
		friend class MeshRenderManager;
		friend class CameraManager;

		Transform() {}
		Transform(float x, float y, float z);
		void translateLocal(float x, float y, float z);
		void translateLocal(glm::vec3& tr_vec);
		void translateDirectionally(float dx, float dy, float dz);
		void translateDirectionally(glm::vec3& tr_vec);
		void translateDirectionallyX(float dx);
		void translateDirectionallyY(float dy);
		void translateDirectionallyZ(float dz);
		void rotate(bool x, bool y, bool z, float angle_in_degrees);
		void set_rotation(float x, float y, float z);
		void set_rotation(glm::vec3& rotate_vec);
		void set_scale(float x, float y, float z);
		void set_scale(glm::vec3& scale_vec);

		void add_child(Component_Ptr<Transform>& child_tr);
		std::vector<Component_Ptr<Transform>>& get_children();
		glm::vec3 position{ 0.f, 0.f, 0.f };
		glm::vec3 rotation{ 0.f, 0.f, 0.f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
	private:
		glm::mat4 worldMatrix{ 1.0f };
		glm::mat4 localMatrix{ 1.0f };
		uint32_t obj_id = 0;
		Component_Ptr<Transform> parent;
		std::vector<Component_Ptr<Transform>> children;
		void update_world_matrix();
		void update(const glm::mat4& worldMatrix);
		void mark_dirty();
		bool isDirty = false;
	};
}
