#pragma once
#include "SubMesh.h"

struct Mesh {
	Mesh() {}
	Mesh(uint32_t id) : id{ id } {}
	uint32_t id{};
	std::vector<uint32_t> subMeshes;
};

class MeshManager {
public:
	static MeshManager* get_Instance() {
		static MeshManager meshMgr;
		return &meshMgr;
	}
	Mesh& add_mesh(Mesh&& mesh);
	Mesh& get_mesh(uint32_t id);
	void delete_mesh(uint32_t id);
private:
	MeshManager() {}
	~MeshManager() {}
	sparse_set<Mesh> meshes;
	std::vector<uint32_t> free_ids;
};
