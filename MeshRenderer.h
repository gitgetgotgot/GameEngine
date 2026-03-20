#pragma once
#include "MeshManager.h"
#include "CompSystemsCore.h"

class MeshRendererSystem;
class MeshRenderManager;

struct MeshUpdater {
	inline static std::vector<uint32_t> dirtyMeshes;
	inline static std::vector<uint32_t> addedMeshes;
	inline static std::vector<std::pair<uint32_t, uint32_t>> deletedMeshes; //first is obj_id, second is mesh_id
private:
	MeshUpdater() {}
	~MeshUpdater() {}
};

namespace Engine {
	namespace Component {
		struct MeshRenderer {
			friend class MeshRendererSystem;
			friend class MeshRenderManager;
			MeshRenderer() : mesh{ nullptr }, obj_id{ 0 } {}
			MeshRenderer(Mesh* mesh, uint32_t obj_id) : mesh{ mesh }, obj_id{ obj_id } {}
			Mesh* mesh;
		private:
			uint32_t obj_id;
		};
	}
}