#pragma once
#include "SparseSet.h"
#include <vector>
#include <glm/glm.hpp>

struct MeshVertex {
	MeshVertex() {}
	MeshVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 UV, float material_id) :
		pos{ pos }, normal{ normal }, UV{ UV }, material_id{ material_id } {}
	glm::vec3 pos{};
	glm::vec3 normal{};
	glm::vec2 UV{};
	float material_id{};
};

struct Mesh {
	Mesh() {}
	uint32_t id;
	std::vector<MeshVertex> vertices;
	std::vector<uint32_t> indices;
};

class MeshManager {
public:
	static MeshManager* get_Instance() {
		static MeshManager meshMgr;
		return &meshMgr;
	}
	void init_basic_meshes();
	void load_mesh();
	void load_meshes();
	Mesh* get_mesh(uint32_t id);
	void delete_mesh(uint32_t id);
private:
	MeshManager() {}
	~MeshManager() {}
	sparse_set<Mesh> meshes;
};
