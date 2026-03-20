#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "CompSystemsCore.h"

class TransformSystem;
class SpriteRenderManager;
class MeshRenderManager;
class CameraManager;

struct TransformUpdater {
	inline static std::vector<uint32_t> dirtyTransforms;
private:
	TransformUpdater() {}
	~TransformUpdater() {}
};

namespace Engine {
	namespace Component {
		struct Transform {
			friend class TransformSystem;
			friend class SpriteRenderManager;
			friend class MeshRenderManager;
			friend class CameraManager;
			
			Transform() {}
			Transform(float x, float y, float z);
			void translateLocal(float x, float y, float z);
			void translateLocal(glm::vec3& tr_vec);
			void translateDirectionally(float x, float y, float z);
			void translateDirectionally(glm::vec3& tr_vec);
			void rotate(bool x, bool y, bool z, float angle_in_degrees);
			void set_rotation(float x, float y, float z);
			void set_rotation(glm::vec3& rotate_vec);
			void set_scale(float x, float y, float z);
			void set_scale(glm::vec3& scale_vec);

			void add_child(component_ptr<Transform>& child_tr, component_ptr<Transform>& parent_tr);
			glm::vec3 position{ 0.f, 0.f, 0.f };
			glm::vec3 rotation{ 0.f, 0.f, 0.f };
			glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		private:
			glm::mat4 worldMatrix{ 1.0f };
			glm::mat4 localMatrix{ 1.0f };
			uint32_t obj_id = 0;
			component_ptr<Transform> parent;
			std::vector<component_ptr<Transform>> children;
			void update_modelMatrix();
			void update(const glm::mat4& worldMatrix);
			void mark_dirty();
			bool isDirty = false;
		};
	}
}
