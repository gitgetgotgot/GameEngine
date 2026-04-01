#include "MaterialManager.h"

Material& MaterialManager::add_material(
	uint32_t albedoID,
	uint32_t normalID,
	uint32_t roughnessID,
	uint32_t metallicID
) {
	uint32_t id;
	if (free_ids.size()) {
		id = free_ids.back();
		free_ids.pop_back();
	}
	else id = materials.size();
	return *materials.add(Material(id, albedoID, normalID, roughnessID, metallicID), id);
}

void MaterialManager::delete_material(uint32_t id) {
	materials.remove(id);
	free_ids.emplace_back(id);
}

Material& MaterialManager::get_material(uint32_t id) {
	return *materials.get(id);
}
