#include "MeshManager.h"

Mesh& MeshManager::add_mesh(Mesh&& mesh) {
    uint32_t id;
    if (free_ids.size()) {
        id = free_ids.back();
        free_ids.pop_back();
    }
    else id = meshes.size();
    mesh.id = id;
    return *meshes.add(std::move(mesh), id);
}

Mesh& MeshManager::get_mesh(uint32_t id) {
	return *meshes.get(id);
}

void MeshManager::delete_mesh(uint32_t id) {
	meshes.remove(id);
    free_ids.emplace_back(id);
}
