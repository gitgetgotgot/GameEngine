#pragma once
#include <Utility/SparseSet.h>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include <iostream> // temporary for logs in systems

namespace Engine::Systems {
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