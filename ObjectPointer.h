#pragma once
#include "Object.h"

namespace Engine {
	namespace Object {
		class object_ptr {
		public:
			object_ptr() {}
			object_ptr(uint32_t obj_id) : obj_id{ obj_id } {}
			uint32_t get_id() const { return obj_id; }
			Object* operator->();
			Object& operator*();
			object_ptr& operator=(Object* obj_ptr);
			operator bool() const;
		private:
			uint32_t obj_id = 0;
		};
	}
}