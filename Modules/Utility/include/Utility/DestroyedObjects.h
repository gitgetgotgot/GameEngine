#include <vector>
#include <cstdint>

namespace Engine::Systems {
	// class that is used in object manager and different component systems
	// (maybe this method should be reworked later to exclude individual delete in all systems)
	class DestroyedObjects {
	public:
		inline static std::vector<uint32_t> destroyed_objects;
	};
}