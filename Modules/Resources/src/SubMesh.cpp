#include <Resources/SubMesh.h>

SubMesh& SubMeshManager::add(SubMesh submesh) {
	uint32_t id;
	if (free_ids.size()) {
		id = free_ids.back();
		free_ids.pop_back();
	}
	else id = subMeshes.size();
	submesh.id = id;
	return *subMeshes.add(std::move(submesh), id);
}

SubMesh& SubMeshManager::get(uint32_t id) {
	return *subMeshes.get(id);
}

void SubMeshManager::remove(uint32_t id) {
	subMeshes.remove(id);
	free_ids.emplace_back(id);
}
