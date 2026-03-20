#pragma once
#include "CompSystemsCore.h"
#include "UI_CompSystemsCore.h"
#include "Transform.h"
#include "UI_Transform.h"

namespace Engine {
	namespace Object {
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
					tr_parent->add_child(tr_child, tr_parent);
					return true;
				}
				return false;
			}
			bool add_child_UI_object(Object& child_UI_object) {
				auto tr_parent = this->get_ui_component<UI::UI_Transform>();
				auto tr_child = child_UI_object.get_ui_component<UI::UI_Transform>();
				if (tr_parent && tr_child) {
					tr_parent->add_child(tr_child, tr_parent);
					return true;
				}
				return false;
			}
			void Destroy() {
				DestroyedObjects::destroyed_objects.emplace_back(id);
			}
			//component methods
			template<typename T, typename ... Args>
			Component::component_ptr<T> add_component(Args&& ... args) {
				IComponentSystem<T>* system = ComponentSystemsCore::get_Instance()->get_system<T>();
				system->add_component(T(std::forward<Args>(args)...), this->id);
				//return system->get_component(this->id);
				return Component::component_ptr<T>(this->id);
			}
			template<typename T>
			Component::component_ptr<T> get_component() {
				IComponentSystem<T>* system = ComponentSystemsCore::get_Instance()->get_system<T>();
				//return system->get_component(this->id);
				return Component::component_ptr<T>(this->id);
			}
			template<typename T>
			void remove_component() {
				IComponentSystem<T>* system = ComponentSystemsCore::get_Instance()->get_system<T>();
				system->remove_component(this->id);
			}
			//UI component methods
			template<typename T, typename ... Args>
			UI::ui_component_ptr<T> add_ui_component(Args&& ... args) {
				UI_IComponentSystem<T>* system = UI_ComponentSystemsCore::get_Instance()->get_system<T>();
				system->add_component(T(std::forward<Args>(args)...), this->id);
				return UI::ui_component_ptr<T>(this->id);
			}
			template<typename T>
			UI::ui_component_ptr<T> get_ui_component() {
				UI_IComponentSystem<T>* system = UI_ComponentSystemsCore::get_Instance()->get_system<T>();
				//return system->get_component(this->id);
				return UI::ui_component_ptr<T>(this->id);
			}
			template<typename T>
			void remove_ui_component() {
				UI_IComponentSystem<T>* system = UI_ComponentSystemsCore::get_Instance()->get_system<T>();
				system->remove_component(this->id);
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
			void remove_script() {
				ObjectManager::get_Instance()->remove_script<T>(this->id);
			}
		private:
			uint32_t id;
		};

	}
}
