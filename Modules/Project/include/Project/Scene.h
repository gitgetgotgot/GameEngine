#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <memory>
#include <Utility/SparseSet.h>
#include <glm/glm.hpp>

enum ComponentType : uint16_t {
	//general
	isTransform, isSpriteComp, isMeshComp, isCamera,
	//UI
	isUI_Transform, isCanvas, isImage, isButton, isSDF_Text
};

//Serializable components data
class ComponentInfo {
public:
	ComponentType type;
};

class TransformInfo : public ComponentInfo {
public:
	glm::vec3 position{ 0.f, 0.f, 0.f };
	glm::vec3 rotation{ 0.f, 0.f, 0.f };
	glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
};

class ObjectInfo {
public:
	uint32_t id;
	std::string name;
	std::vector<std::unique_ptr<ComponentInfo>> components;
};

class SceneInfo {
public:
	std::filesystem::path scene_path;
};

class Scene {
public:
	std::filesystem::path scene_path;
	sparse_set<ObjectInfo> objects;
private:
};