#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <vector>
#include <UI/UIComponentSystems/UI_CompSystemsCore.h>

struct UI_TransformUpdater {
	inline static std::vector<uint32_t> dirtyTransforms;
private:
	UI_TransformUpdater() {}
	~UI_TransformUpdater() {}
};

namespace Engine::Systems {
	class UI_TransformSystem;
}

namespace Engine::UI {
	class UI_Transform {
	public:
		friend class Engine::Systems::UI_TransformSystem;

		UI_Transform() {}
		UI_Transform(float x, float y, float z);
		void translateLocal(float x, float y, float z);
		void translateLocal(glm::vec3& tr_vec);
		void translateDirectionally(float x, float y, float z);
		void translateDirectionally(glm::vec3& tr_vec);
		void rotate(bool x, bool y, bool z, float angle_in_degrees);
		void set_rotation(float x, float y, float z);
		void set_rotation(glm::vec3& rotate_vec);
		void set_scale(float x, float y);
		void set_scale(glm::vec2& scale_vec);
		void set_size(float x, float y);
		void set_size(glm::vec2& size_vec);
		void set_pivot(float x, float y);
		void set_pivot(glm::vec2& pivot_vec);

		void add_child(Engine::UI::UI_Component_Ptr<UI_Transform>& child_tr);
		std::vector<UI_Component_Ptr<UI_Transform>>& get_children();
		glm::mat4 get_final_ui_matrix() const;
		glm::mat4 get_world_matrix() const;
		glm::vec3 position{ 0.f, 0.f, 0.f };
		glm::vec3 rotation{ 0.f, 0.f, 0.f };
		glm::vec2 size{ 1.f, 1.f };
		glm::vec2 pivot{ 0.5f, 0.5f };
		glm::vec2 scale{ 1.0f, 1.0f };
	private:
		glm::mat4 worldMatrix{ 1.0f };
		glm::mat4 localMatrix{ 1.0f };
		uint32_t obj_id = 0;
		Engine::UI::UI_Component_Ptr<UI_Transform> parent;
		std::vector<Engine::UI::UI_Component_Ptr<UI_Transform>> children;
		void update_world_matrix();
		void update(const glm::mat4& worldMatrix);
		void mark_dirty();
		bool isDirty = false;
	};
}
