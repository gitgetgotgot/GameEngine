#pragma once
#include "Scene.h"

class SceneManager {
public:
	static SceneManager* get_Instance() {
		static SceneManager sceneMgr;
		return &sceneMgr;
	}
	bool LoadScene(std::string& scene_path);
	void CreateScene(std::filesystem::path& scene_path);
	void SaveScene();
	std::unique_ptr<Scene>& get_current_scene();

	void add_object(uint32_t obj_id);
	void remove_object(uint32_t obj_id);
private:
	SceneManager() {};
	~SceneManager() {};
	std::unique_ptr<Scene> current_scene;
};