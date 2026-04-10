#pragma once
#include "MeshManager.h"
#include "CompSystemsCore.h"
#include "Transform.h"

namespace Engine::Systems {
	class MeshComponentSystem;
}

class MeshRenderManager;

struct MeshUpdater {
	inline static std::vector<uint32_t> dirtyMeshes;
	inline static std::vector<uint32_t> addedMeshes;
	inline static std::vector<std::pair<uint32_t, uint32_t>> deletedMeshes; //first is obj_id, second is mesh_id
private:
	MeshUpdater() {}
	~MeshUpdater() {}
};

namespace Engine::Component {
	class MeshComponent {
	public:
		friend class Engine::Systems::MeshComponentSystem;
		friend class MeshRenderManager;
		MeshComponent() {}
		MeshComponent(uint32_t mesh_id, uint32_t obj_id) : mesh_id{ mesh_id }, obj_id{ obj_id } {}
		uint32_t mesh_id{};
	private:
		uint32_t obj_id{};
		Component_Ptr<Component::Transform> transform_ptr;
	};
}