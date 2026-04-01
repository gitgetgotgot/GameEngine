#pragma once
#include "SparseSet.h"
#include "SpriteManager.h"
#include <unordered_map>
#include <typeindex>
#include <memory>

#include <iostream> // temporary for logs in systems

namespace Engine::Systems {
	// struct that is used in object manager and different component systems
	// (maybe this method should be reworked later to exclude individual delete in all systems)
	struct DestroyedObjects {
		inline static std::vector<uint32_t> destroyed_objects;
	};

	class ISystem {
	public:
		virtual ~ISystem() {}
		virtual void update() = 0;
	};

	template<typename Comp>
	class IComponentSystem : public ISystem {
	public:
		virtual void add_component(Comp&& comp, uint32_t obj_id) = 0;
		virtual Comp* get_component(uint32_t obj_id) = 0;
		virtual bool remove_component(uint32_t obj_id) = 0;
	};

	class ComponentSystemsCore;

	struct UI_InstanceData {
		UI_InstanceData() {}
		UI_InstanceData(Sprite& sprite, glm::mat4& mat) {
			modelMatrix = mat;
			UV[0] = sprite.UV[0]; UV[1] = sprite.UV[1]; UV[2] = sprite.UV[2]; UV[3] = sprite.UV[3];
		}
		UI_InstanceData(const UI_InstanceData&) = default;
		UI_InstanceData(UI_InstanceData&&) = default;
		glm::mat4 modelMatrix{};
		glm::vec2 UV[4]{};
		glm::vec4 color{};
		uint64_t handle{};
	};

	class UI_ISystem {
	public:
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
}

namespace Engine::Component {
	template<typename T>
	class Component_Ptr {
		friend class Engine::Systems::ComponentSystemsCore;
	public:
		Component_Ptr() {}
		Component_Ptr(uint32_t obj_id) : obj_id{ obj_id } {}
		uint32_t get_object_id() const { return obj_id; }
		T* operator->();
		T& operator*();
		Component_Ptr<T>& operator=(Component_Ptr<T> component_ptr);
		operator bool() const;
	private:
		uint32_t obj_id = 0; //0 is nullptr
		inline static Engine::Systems::IComponentSystem<T>* component_system_ptr = nullptr;
	};
	template<typename T>
	inline T* Component_Ptr<T>::operator->() {
		return component_system_ptr->get_component(obj_id);
	}
	template<typename T>
	inline T& Component_Ptr<T>::operator*() {
		return *component_system_ptr->get_component(obj_id);
	}
	template<typename T>
	inline Component_Ptr<T>& Component_Ptr<T>::operator=(Component_Ptr<T> component_ptr) {
		this->obj_id = component_ptr.obj_id;
		return *this;
	}
	template<typename T>
	inline Component_Ptr<T>::operator bool() const {
		return component_system_ptr->get_component(obj_id) && obj_id > 0;
	}
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
