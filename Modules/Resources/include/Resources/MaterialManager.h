#pragma once
#include <cstdint>
#include <vector>
#include <Rendering/Interfaces/Renderer.h>

constexpr uint32_t MAX_MATERIALS = 5000;

struct Material {
	Material() {}
	Material(
		uint32_t id = 0,
		uint32_t albedoID = 0,
		uint32_t normalID = 0,
		uint32_t roughnessID = 0,
		uint32_t metallicID = 0
	) : id{ id },
		albedoID{ albedoID },
		normalID{ normalID },
		roughnessID{ roughnessID },
		metallicID{metallicID} {}
	Material(const Material&) = default;
	Material& operator=(const Material&) = default;
	Material(Material&&) = default;
	Material& operator=(Material&&) = default;

	uint32_t id = 0;
	uint32_t albedoID = 0;
	uint32_t normalID = 0;
	uint32_t roughnessID = 0;
	uint32_t metallicID = 0;
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
	void change_material(
		uint32_t id,
		uint32_t albedoID,
		uint32_t normalID,
		uint32_t roughnessID,
		uint32_t metallicID);
	void init(std::unique_ptr<Engine::Graphics::RendererInterface>& renderer);
private:
	MaterialManager() {}
	~MaterialManager() {}
	std::unique_ptr<StorageBuffer> materialsBuffer;
	std::vector<Material> materials;
	uint32_t current_max_size = 0;
	std::vector<uint32_t> free_ids;
};