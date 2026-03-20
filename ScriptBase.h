#pragma once
#include "ObjectPointer.h"
#include "ScriptsMacros.h"

namespace Engine {
	namespace Object {
		class ScriptBase {
		public:
			ScriptBase() {}
			virtual ~ScriptBase() {}
			//basic methods
			virtual void Update() {}
			virtual void LateUpdate() {}
			virtual void OnConstruct() {}
			virtual void OnEnable() {}
			virtual void OnDisable() {}
			virtual void OnDestroy() {}
		public:
			object_ptr object;
		};
	}
}
