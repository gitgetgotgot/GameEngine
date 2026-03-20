#pragma once
#include "ScriptBase.h"

namespace Engine {
	namespace Object {
		
		class ScriptManager {
		public:
			static ScriptManager* get_Instance() {
				static ScriptManager scriptMgr;
				return &scriptMgr;
			}
			void Update();
			//object script methods
			template<typename T, typename ... Args>
			T* add_script(uint32_t obj_id, Args&& ... args);
			template<typename T>
			T* get_script(uint32_t obj_id);
			template<typename T>
			void remove_script(uint32_t obj_id);
		private:
			ScriptManager() {}
			~ScriptManager() {}
			//scripts
			sparse_set<std::unique_ptr<ScriptBase>> scripts;
			std::vector<ScriptBase*> scripts_with_Update;
			std::vector<ScriptBase*> scripts_with_LateUpdate;
		};


	}
}