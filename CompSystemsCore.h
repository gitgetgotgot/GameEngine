#pragma once
#include "CompSystemInterfaces.h"

namespace Engine::Systems {
	class ComponentSystemsCore {
	public:
		static ComponentSystemsCore* get_Instance() {
			static ComponentSystemsCore system;
			return &system;
		}
		template<typename Comp, typename System>
		void register_system() {
			//create new Comp system
			systems.emplace(std::type_index(typeid(Comp)), std::make_unique<System>());
			//set this system to component_ptr static field
			Engine::Component::Component_Ptr<Comp>::component_system_ptr = get_system<Comp>();
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
}

