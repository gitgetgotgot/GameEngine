#pragma once
#include <Utility/SparseSet.h>
#include <Resources/SpriteManager.h>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include <iostream> // temporary for logs in systems

namespace Engine::Systems {

	struct UI_InstanceData {
		UI_InstanceData() {}
		UI_InstanceData(Sprite& sprite, glm::mat4& mat) {
			modelMatrix = mat;
			UV[0] = sprite.UV[0]; UV[1] = sprite.UV[1]; UV[2] = sprite.UV[2]; UV[3] = sprite.UV[3];
		}
		glm::mat4 modelMatrix{ 1.0f };
		glm::vec2 UV[4]{};
		glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };
		uint32_t texture_id{ 0 };
		uint32_t padding[3]{};
	};

	enum UI_System_Type : uint8_t {
		isNotText, isText
	};

	class UI_ISystem {
	public:
		UI_System_Type type = isNotText;
		virtual ~UI_ISystem() {}
		virtual void update() = 0;
		virtual void update_ui_buffer_data(std::vector<UI_InstanceData>& ui_data, uint32_t obj_id) = 0;
	};

	template<typename UI_Comp>
	class UI_IComponentSystem : public UI_ISystem {
	public:
		virtual void add_component(UI_Comp&& comp, uint32_t obj_id) = 0;
		virtual UI_Comp* get_component(uint32_t obj_id) = 0;
		virtual bool remove_component(uint32_t obj_id) = 0;
	};

	class UI_ComponentSystemsCore;

	class UI_Components_Data_Access {
	public:
		inline static std::unordered_map<uint32_t, Engine::Systems::UI_ISystem*> comp_objects;
	};
}

namespace Engine::UI {
	template<typename T>
	class UI_Component_Ptr {
		friend class Engine::Systems::UI_ComponentSystemsCore;
	public:
		UI_Component_Ptr() {};
		UI_Component_Ptr(uint32_t obj_id) : obj_id{ obj_id } {}
		uint32_t get_object_id() const { return obj_id; }
		T* operator->();
		T& operator*();
		UI_Component_Ptr<T>& operator=(UI_Component_Ptr<T> component_ptr);
		operator bool() const;
	private:
		uint32_t obj_id = 0; //0 is nullptr
		inline static Engine::Systems::UI_IComponentSystem<T>* component_system_ptr = nullptr;
	};
	template<typename T>
	inline T* UI_Component_Ptr<T>::operator->() {
		return component_system_ptr->get_component(obj_id);
	}
	template<typename T>
	inline T& UI_Component_Ptr<T>::operator*() {
		return *component_system_ptr->get_component(obj_id);
	}
	template<typename T>
	inline UI_Component_Ptr<T>& UI_Component_Ptr<T>::operator=(UI_Component_Ptr<T> component_ptr) {
		this->obj_id = component_ptr.obj_id;
		return *this;
	}
	template<typename T>
	inline UI_Component_Ptr<T>::operator bool() const {
		return component_system_ptr->get_component(obj_id) && obj_id > 0;
	}
}
