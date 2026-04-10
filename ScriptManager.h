#pragma once
#include "ScriptBase.h"

namespace Engine::Object {
	class ScriptManager {
	public:
		static ScriptManager* get_Instance() {
			static ScriptManager scriptMgr;
			return &scriptMgr;
		}
		void _internal_init();
		void _internal_update();
		//object script methods
		template<typename T, typename ... Args>
		T* add_script(uint32_t obj_id, Args&& ... args);
		template<typename T>
		T* get_script(uint32_t obj_id);
		template<typename T>
		bool remove_script(uint32_t obj_id);
	private:
		ScriptManager() {}
		~ScriptManager() {}
		//scripts
		sparse_set<std::unique_ptr<ScriptBase>> scripts;
		std::vector<uint32_t> scripts_with_Update;
		std::vector<uint32_t> scripts_with_LateUpdate;
	};
	template<typename T, typename ...Args>
	inline T* ScriptManager::add_script(uint32_t obj_id, Args && ...args) {
		Engine::Object::ScriptBase* script = scripts.add(std::make_unique<T>(std::forward<Args>(args)...), obj_id)->get();
		script->object = object_ptr(obj_id);

		if constexpr (requires { T::hasUpdate; }) {
			scripts_with_Update.emplace_back(obj_id);
		}
		if constexpr (requires { T::hasLateUpdate; }) {
			scripts_with_LateUpdate.emplace_back(obj_id);
		}
		if constexpr (requires { T::hasOnConstruct; }) {
			script->OnConstruct();
		}

		return static_cast<T*>(script);
	}
	template<typename T>
	inline T* ScriptManager::get_script(uint32_t obj_id) {
		return static_cast<T*>(scripts.get(obj_id)->get());
	}
	template<typename T>
	inline bool ScriptManager::remove_script(uint32_t obj_id) {
		return scripts.remove(obj_id);
	}
}