#pragma once
#include <cstdint>
#include "SparseSet.h"

struct Material {
	Material() {}
	Material(
		uint32_t id,
		uint32_t albedoID,
		uint32_t normalID,
		uint32_t roughnessID,
		uint32_t metallicID
	) : id{ id },
		albedoID{ albedoID },
		normalID{ normalID },
		roughnessID{ roughnessID },
		metallicID{metallicID} { }
	uint32_t id{};
	uint32_t albedoID{};
	uint32_t normalID{};
	uint32_t roughnessID{};
	uint32_t metallicID{};
};

class MaterialManager {
public:
	static MaterialManager* get_Instance() {
		static MaterialManager modelLoader;
		return &modelLoader;
	}
	Material& add_material(
		uint32_t albedoID = 0,
		uint32_t normalID = 0,
		uint32_t roughnessID = 0,
		uint32_t metallicID = 0);
	void delete_material(uint32_t id);
	Material& get_material(uint32_t id);
private:
	MaterialManager() {}
	~MaterialManager() {}
	sparse_set<Material> materials;
	std::vector<uint32_t> free_ids;
};