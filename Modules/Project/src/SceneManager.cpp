#include <Project/SceneManager.h>

bool SceneManager::LoadScene(std::string& scene_path) {
	current_scene = std::make_unique<Scene>();
	current_scene->scene_path = scene_path;
    return true;
}

void SceneManager::CreateScene(std::filesystem::path& scene_path) {
	std::ofstream sceneFile(scene_path, std::ios::binary);

	uint32_t objects_count = 0;
	sceneFile.write(reinterpret_cast<const char*>(&objects_count), sizeof(objects_count));

	sceneFile.close();
}

void SceneManager::SaveScene() {
	std::ofstream sceneFile(current_scene->scene_path, std::ios::binary);

	uint32_t objects_count = current_scene->objects.size();
	sceneFile.write(reinterpret_cast<const char*>(&objects_count), sizeof(objects_count));

	ComponentType type;
	for (uint32_t i = 0; i < objects_count; i++) {

		std::vector<std::unique_ptr<ComponentInfo>>& comps_info = current_scene->objects[i].components;
		uint32_t components_count = comps_info.size();
		sceneFile.write(reinterpret_cast<const char*>(&components_count), sizeof(components_count));

		for (auto& comp : comps_info) {

			sceneFile.write(reinterpret_cast<const char*>(&comp->type), sizeof(comp->type));

			switch (comp->type) {
				case ComponentType::isTransform: {
					TransformInfo* tr = static_cast<TransformInfo*>(comp.get());
					sceneFile.write(reinterpret_cast<const char*>(&tr->position), sizeof(tr->position));
					sceneFile.write(reinterpret_cast<const char*>(&tr->rotation), sizeof(tr->rotation));
					sceneFile.write(reinterpret_cast<const char*>(&tr->scale), sizeof(tr->scale));
					break;
				}
				case ComponentType::isSpriteComp: {

					break;
				}
				case ComponentType::isMeshComp: {

					break;
				}
				case ComponentType::isCamera: {

					break;
				}
			}

		}

	}

	sceneFile.close();
}

std::unique_ptr<Scene>& SceneManager::get_current_scene() {
	return current_scene;
}

void SceneManager::add_object(uint32_t obj_id) {
	ObjectInfo info;
	info.id = obj_id;
	current_scene->objects.add(std::move(info), obj_id);
}

void SceneManager::remove_object(uint32_t obj_id) {
	current_scene->objects.remove(obj_id);
}
