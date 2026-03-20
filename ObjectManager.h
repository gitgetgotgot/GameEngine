#pragma once
#include "ScriptBase.h"
#include <iostream>

namespace Engine {
	namespace Object {

		class ObjectManager {
			friend class object_ptr;
		public:
			static ObjectManager* get_Instance() {
				static ObjectManager objMgr;
				return &objMgr;
			}
			template<typename...Args>
			object_ptr InstantiateObject(Args&& ... args);
			void DestroyObject(uint32_t obj_id);
			void DestroyObject(object_ptr& object);
			void Update();
			Object* get_object(uint32_t id);
			//object script methods
			template<typename T, typename ... Args>
			T* add_script(uint32_t obj_id, Args&& ... args);
			template<typename T>
			T* get_script(uint32_t obj_id);
			template<typename T>
			void remove_script(uint32_t obj_id);
		private:
			ObjectManager() {
				//objects.reserve(10);
			}
			~ObjectManager() {}
			//objects
			sparse_set<Object> objects;
			uint32_t current_id = 1; //first id is 1
			//scripts
			sparse_set<std::unique_ptr<ScriptBase>> scripts;
			std::vector<ScriptBase*> scripts_with_Update;
			std::vector<ScriptBase*> scripts_with_LateUpdate;
		};

		template<typename ...Args>
		inline object_ptr ObjectManager::InstantiateObject(Args&& ...args) {
			objects.add(Object(current_id), current_id);
			return object_ptr(current_id++);
		}
		template<typename T, typename ... Args>
		inline T* ObjectManager::add_script(uint32_t obj_id, Args&& ... args) {
			Engine::Object::ScriptBase* script = scripts.add(std::make_unique<T>(std::forward<Args>(args)...), obj_id)->get();
			script->object = objects.get(obj_id);

			if constexpr (requires { T::hasUpdate; }) {
				scripts_with_Update.emplace_back(script);
			}
			if constexpr (requires { T::hasLateUpdate; }) {
				scripts_with_LateUpdate.emplace_back(script);
			}
			if constexpr (requires { T::hasOnConstruct; }) {
				script->OnConstruct();
			}

			return static_cast<T*>(script);
		}
		template<typename T>
		inline T* ObjectManager::get_script(uint32_t obj_id) {
			return static_cast<T*>(scripts.get(obj_id)->get());
		}
		template<typename T>
		inline void ObjectManager::remove_script(uint32_t obj_id) {
			scripts.remove(obj_id);
		}

	}
}
