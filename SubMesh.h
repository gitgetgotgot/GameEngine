#pragma once
#include "SparseSet.h"
#include <vector>
#include <glm/glm.hpp>

struct SubMeshVertex {
	SubMeshVertex() {}
	SubMeshVertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 UV) :
		pos{ pos }, normal{ normal }, UV{ UV } {
	}
	glm::vec3 pos{};
	glm::vec3 normal{};
	glm::vec2 UV{};
	//glm::vec4 tangent{};
};

struct SubMesh {
	SubMesh() {}
	SubMesh(uint32_t indexCount, uint32_t vertexCount) : indexCount{ indexCount }, vertexCount{ vertexCount } {}
	uint32_t id{};
	uint32_t material_id{};
	//render data
	uint32_t indexCount{};  //total amount of indices for this mesh
	uint32_t vertexCount{}; //total amount of vertices for this mesh
	uint32_t baseIndex{};   //first index of this mesh in global index buffer
	uint32_t baseVertex{};  //first vertex of this mesh in global vertex buffer
};

class SubMeshManager {
public:
	static SubMeshManager* get_Instance() {
		static SubMeshManager submeshMgr;
		return &submeshMgr;
	}
	SubMesh& add(SubMesh submesh);
	SubMesh& get(uint32_t id);
	void remove(uint32_t id);
private:
	SubMeshManager() {}
	~SubMeshManager() {}
	sparse_set<SubMesh> subMeshes;
	std::vector<uint32_t> free_ids;
};

