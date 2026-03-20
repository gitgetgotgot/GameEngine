#pragma once
#include "CompSystemInterfaces.h"

class ComponentSystemsCore {
public:
	static ComponentSystemsCore* get_Instance() {
		static ComponentSystemsCore system;
		return &system;
	}
	template<typename Comp, typename System>
	void register_system() {
		systems.emplace(std::type_index(typeid(Comp)), std::make_unique<System>());
	}
	template<typename Comp>
	IComponentSystem<Comp>* get_system() {
		auto it = systems.find(std::type_index(typeid(Comp)));
		if (it != systems.end()) {
			return static_cast<IComponentSystem<Comp>*>(it->second.get());
		}
		return nullptr;
	}
	void update() {
		for (auto& s : systems) {
			s.second->update();
		}
	}
private:
	std::unordered_map<std::type_index, std::unique_ptr<ISystem>> systems;
	ComponentSystemsCore() {}
	~ComponentSystemsCore() {}
};

namespace Engine {
	namespace Component {
		template<typename T>
		class component_ptr {
		public:
			component_ptr();
			component_ptr(uint32_t obj_id);
			uint32_t get_object_id() const { return obj_id; }
			T* operator->();
			T& operator*();
			component_ptr<T>& operator=(component_ptr<T> component_ptr);
			operator bool() const;
		private:
			uint32_t obj_id = 0; //0 is nullptr
			IComponentSystem<T>* component_system_ptr;
		};

		template<typename T>
		inline component_ptr<T>::component_ptr() {
			component_system_ptr = ComponentSystemsCore::get_Instance()->get_system<T>();
		}
		template<typename T>
		inline component_ptr<T>::component_ptr(uint32_t obj_id) {
			component_system_ptr = ComponentSystemsCore::get_Instance()->get_system<T>();
			this->obj_id = obj_id;
		}
		template<typename T>
		inline T* component_ptr<T>::operator->() {
			return component_system_ptr->get_component(obj_id);
		}
		template<typename T>
		inline T& component_ptr<T>::operator*() {
			return *component_system_ptr->get_component(obj_id);
		}
		template<typename T>
		inline component_ptr<T>& component_ptr<T>::operator=(component_ptr<T> component_ptr) {
			this->obj_id = component_ptr.obj_id;
			this->component_system_ptr = component_ptr.component_system_ptr;
			return *this;
		}
		template<typename T>
		inline component_ptr<T>::operator bool() const {
			return component_system_ptr->get_component(obj_id) && obj_id > 0; //probably not correct, should check ptr first
		}
	}
}