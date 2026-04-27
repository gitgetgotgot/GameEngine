#include <Resources/MaterialManager.h>

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
	else {
		id = current_max_size;
		current_max_size++;
		materials.emplace_back(Material(0));
	}
	materials[id] = Material(id, albedoID, normalID, roughnessID, metallicID);

	//update materials global storage buffer
	materialsBuffer->updateData(&materials[id], sizeof(Material), id * sizeof(Material));

	return materials[id];
}

void MaterialManager::delete_material(uint32_t id) {
	if (id >= current_max_size) return;
	materials[id] = Material(0);
	free_ids.emplace_back(id);

	//update materials global storage buffer
	materialsBuffer->updateData(&materials[id], sizeof(Material), id * sizeof(Material));
}

Material& MaterialManager::get_material(uint32_t id) {
	if (id >= current_max_size) return materials[0];
	return materials[id];
}

void MaterialManager::change_material(uint32_t id, uint32_t albedoID, uint32_t normalID, uint32_t roughnessID, uint32_t metallicID) {
	if (id >= current_max_size) return;
	Material& mat = materials[id];
	mat.albedoID = albedoID;
	mat.normalID = normalID;
	mat.roughnessID = roughnessID;
	mat.metallicID = metallicID;
	materialsBuffer->updateData(&mat, sizeof(Material), id * sizeof(Material));
}

void MaterialManager::init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer) {
	materialsBuffer = renderer->createStorageBuffer();
	materialsBuffer->bind(4);
	materialsBuffer->setData(nullptr, MAX_MATERIALS * sizeof(Material));

	materials.reserve(MAX_MATERIALS);
}