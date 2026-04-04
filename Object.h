#pragma once
#include "CompSystemsCore.h"
#include "UI_CompSystemsCore.h"
#include "Transform.h"
#include "UI_Transform.h"

namespace ES = Engine::Systems;

namespace Engine::Object {
	class ObjectManager;

	class Object {
	public:
		Object(uint32_t id) { this->id = id; }
		~Object() {}
		uint32_t get_id() { return id; }
		bool add_child_object(Object& child_object) {
			auto tr_parent = this->get_component<Component::Transform>();
			auto tr_child = child_object.get_component<Component::Transform>();
			if (tr_parent && tr_child) {
				tr_parent->add_child(tr_child);
				return true;
			}
			return false;
		}
		bool add_child_UI_object(Object& child_UI_object) {
			auto tr_parent = this->get_ui_component<UI::UI_Transform>();
			auto tr_child = child_UI_object.get_ui_component<UI::UI_Transform>();
			if (tr_parent && tr_child) {
				tr_parent->add_child(tr_child);
				return true;
			}
			return false;
		}
		void Destroy() {
			Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(id);
			auto transform = get_component<Component::Transform>();
			if (transform) {
				std::vector<Component::Component_Ptr<Component::Transform>>& children = transform->get_children();
				for(auto& c : children)
					Engine::Systems::DestroyedObjects::destroyed_objects.emplace_back(c.get_object_id());
			}
		}
		//component methods
		template<typename T, typename ... Args>
		Component::Component_Ptr<T> add_component(Args&& ... args) {
			ES::IComponentSystem<T>* system = ES::ComponentSystemsCore::get_Instance()->get_system<T>();
			system->add_component(T(std::forward<Args>(args)...), this->id);
			return Component::Component_Ptr<T>(this->id);
		}
		template<typename T>
		Component::Component_Ptr<T> get_component() {
			return Component::Component_Ptr<T>(this->id);
		}
		template<typename T>
		bool remove_component() {
			ES::IComponentSystem<T>* system = ES::ComponentSystemsCore::get_Instance()->get_system<T>();
			return system->remove_component(this->id);
		}
		//UI component methods
		template<typename T, typename ... Args>
		UI::UI_Component_Ptr<T> add_ui_component(Args&& ... args) {
			ES::UI_IComponentSystem<T>* system = ES::UI_ComponentSystemsCore::get_Instance()->get_system<T>();
			system->add_component(T(std::forward<Args>(args)...), this->id);
			return UI::UI_Component_Ptr<T>(this->id);
		}
		template<typename T>
		UI::UI_Component_Ptr<T> get_ui_component() {
			return UI::UI_Component_Ptr<T>(this->id);
		}
		template<typename T>
		bool remove_ui_component() {
			ES::UI_IComponentSystem<T>* system = ES::UI_ComponentSystemsCore::get_Instance()->get_system<T>();
			return system->remove_component(this->id);
		}
		//script methods
		template<typename T, typename ... Args>
		T* add_script(Args&& ... args) {
			return ObjectManager::get_Instance()->add_script<T>(this->id, std::forward<Args>(args)...);
		}
		template<typename T>
		T* get_script() {
			return ObjectManager::get_Instance()->get_script<T>(this->id);
		}
		template<typename T>
		bool remove_script() {
			return ObjectManager::get_Instance()->remove_script<T>(this->id);
		}
	private:
		uint32_t id;
	};
}