#pragma once
#include "CompSystemInterfaces.h"

namespace Engine::Systems {
	class UI_ComponentSystemsCore {
	public:
		static UI_ComponentSystemsCore* get_Instance() {
			static UI_ComponentSystemsCore system;
			return &system;
		}
		template<typename Comp, typename System>
		void register_system() {
			//create new Comp system
			systems.emplace(std::type_index(typeid(Comp)), std::make_unique<System>());
			//set this system to ui_component_ptr static field
			Engine::UI::UI_Component_Ptr<Comp>::component_system_ptr = get_system<Comp>();
		}
		template<typename Comp>
		UI_IComponentSystem<Comp>* get_system() {
			auto it = systems.find(std::type_index(typeid(Comp)));
			if (it != systems.end()) {
				return static_cast<UI_IComponentSystem<Comp>*>(it->second.get());
			}
			return nullptr;
		}
		template<typename Comp>
		UI_ISystem* get_system_interface() {
			auto it = systems.find(std::type_index(typeid(Comp)));
			if (it != systems.end()) {
				return it->second.get();
			}
			return nullptr;
		}
		void update() {
			for (auto& s : systems) {
				s.second->update();
			}
		}
	private:
		std::unordered_map<std::type_index, std::unique_ptr<UI_ISystem>> systems;
		UI_ComponentSystemsCore() {}
		~UI_ComponentSystemsCore() {}
	};
}