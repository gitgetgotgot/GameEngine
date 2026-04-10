#pragma once
#include "ScriptManager.h"

namespace Engine::Object {
	class ObjectManager {
		friend class object_ptr;
	public:
		static ObjectManager* get_Instance() {
			static ObjectManager objMgr;
			return &objMgr;
		}
		void _internal_init();
		void _internal_update();
		template<typename...Args>
		object_ptr InstantiateObject(Args&& ... args);
		void DestroyObject(uint32_t obj_id);
		void DestroyObject(object_ptr& object);
		Object* get_object(uint32_t id);
	private:
		ObjectManager() {}
		~ObjectManager() {}
		//objects
		sparse_set<Object> objects;
		std::vector<uint32_t> free_ids;
		uint32_t current_max_id = 1; //first id is 1
	};

	template<typename ...Args>
	inline object_ptr ObjectManager::InstantiateObject(Args&& ...args) {
		uint32_t id;
		if (free_ids.size()) {
			id = free_ids.back();
			free_ids.pop_back();
		}
		else id = current_max_id++;
		objects.add(Object(id), id);
		return object_ptr(id);
	}
}
