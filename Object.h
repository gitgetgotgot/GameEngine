#pragma once
#include "CompSystemsCore.h"
#include "UI_CompSystemsCore.h"
#include "Transform.h"
#include "UI_Transform.h"

namespace ES = Engine::Systems;

namespace Engine::Object {
	class ObjectManager;
	class ScriptManager;

	class Object;

	class object_ptr {
		friend class ObjectManager;
	public:
		object_ptr() {}
		object_ptr(uint32_t obj_id) : obj_id{ obj_id } {}
		uint32_t get_id() const { return obj_id; }
		Object* operator->();
		Object& operator*();
		object_ptr& operator=(Object* obj_ptr);
		object_ptr& operator=(object_ptr obj_ptr);
		operator bool() const;
	private:
		uint32_t obj_id = 0;
		inline static ObjectManager* object_manager_ptr = nullptr;
	};

	template<typename T>
	class script_ptr {
		friend class ScriptManager;
	public:
		script_ptr() {}
		script_ptr(uint32_t obj_id) : obj_id{ obj_id } {}
		uint32_t get_id() const { return obj_id; }
		T* operator->();
		T& operator*();
		script_ptr<T>& operator=(script_ptr<T> scriptPtr) {
			this->obj_id = scriptPtr.obj_id;
			return *this;
		}
		operator bool() const;
	private:
		uint32_t obj_id = 0;
	};

	class Object {
	public:
		Object(uint32_t id) : id{ id } {}
		~Object() {}
		uint32_t get_id() const { return id; }
		bool add_child_object(object_ptr& child_object);
		bool add_child_UI_object(object_ptr& child_UI_object);
		void Destroy();
		//component methods
		template<typename T, typename ... Args> Component::Component_Ptr<T> add_component(Args&& ... args);
		template<typename T> Component::Component_Ptr<T> get_component();
		template<typename T> bool remove_component();
		//UI component methods
		template<typename T, typename ... Args> UI::UI_Component_Ptr<T> add_ui_component(Args&& ... args);
		template<typename T> UI::UI_Component_Ptr<T> get_ui_component();
		template<typename T> bool remove_ui_component();
		//script methods
		template<typename T, typename ... Args> script_ptr<T> add_script(Args&& ... args);
		template<typename T> script_ptr<T> get_script();
		template<typename T> bool remove_script();
	private:
		uint32_t id;
	};

	template<typename T, typename ...Args>
	inline Component::Component_Ptr<T> Object::add_component(Args && ...args) {
		ES::IComponentSystem<T>* system = ES::ComponentSystemsCore::get_Instance()->get_system<T>();
		system->add_component(T(std::forward<Args>(args)...), this->id);
		return Component::Component_Ptr<T>(this->id);
	}
	template<typename T>
	inline Component::Component_Ptr<T> Object::get_component() {
		return Component::Component_Ptr<T>(this->id);
	}
	template<typename T>
	inline bool Object::remove_component() {
		ES::IComponentSystem<T>* system = ES::ComponentSystemsCore::get_Instance()->get_system<T>();
		return system->remove_component(this->id);
	}
	template<typename T, typename ...Args>
	inline UI::UI_Component_Ptr<T> Object::add_ui_component(Args && ...args) {
		ES::UI_IComponentSystem<T>* system = ES::UI_ComponentSystemsCore::get_Instance()->get_system<T>();
		system->add_component(T(std::forward<Args>(args)...), this->id);
		return UI::UI_Component_Ptr<T>(this->id);
	}
	template<typename T>
	inline UI::UI_Component_Ptr<T> Object::get_ui_component() {
		return UI::UI_Component_Ptr<T>(this->id);
	}
	template<typename T>
	inline bool Object::remove_ui_component() {
		ES::UI_IComponentSystem<T>* system = ES::UI_ComponentSystemsCore::get_Instance()->get_system<T>();
		return system->remove_component(this->id);
	}
	template<typename T, typename ...Args>
	inline script_ptr<T> Object::add_script(Args && ...args) {
		ScriptManager::get_Instance()->add_script<T>(this->id, std::forward<Args>(args)...);
		return script_ptr<T>(this->id);
	}
	template<typename T>
	inline script_ptr<T> Object::get_script() {
		return script_ptr<T>(this->id);
	}
	template<typename T>
	inline bool Object::remove_script() {
		return ScriptManager::get_Instance()->remove_script<T>(this->id);
	}

	template<typename T>
	inline T* script_ptr<T>::operator->() {
		return ScriptManager::get_Instance()->get_script<T>(this->obj_id);
	}
	template<typename T>
	inline T& script_ptr<T>::operator*() {
		return *ScriptManager::get_Instance()->get_script<T>(this->obj_id);
	}
	template<typename T>
	inline script_ptr<T>::operator bool() const {
		return ScriptManager::get_Instance()->get_script<T>(this->obj_id);
	}
}